/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
// This file contains the memory allocation routines use by this port to
// the Big Number library.

//** Includes and Defines

#define COLLECT_METRICS

#include "Tpm.h"

#if MATH_LIB == MSBN

#ifdef COLLECT_METRICS

memoryMetric memoryMetrics[MAX_METRICS] = {0};

//*** FindMetric()
// This is called when a context is created for a function. It searches to find the
// function name and if found, it associates the context address with the name. This
// allows the entry to be found using the
memoryMetric *
FindMetric(
    LPCSTR           name
    )
{
    int              i;
    for(i = 0; i < MAX_METRICS; i++)
    {
        if(memoryMetrics[i].name == NULL)
        {
            memoryMetrics[i].name = name;
            break;
        }
        if(strcmp(name, memoryMetrics[i].name) == 0)
            break;
    }
    pAssert(i < MAX_METRICS);
    return &memoryMetrics[i];
}

#endif

// The context structures are contained in the caller's space. This function links
// and initializes them.
//
//*** MsBnContextStart()
pMsbnCtx
MsBnContextStart(
    LPCSTR               name,
    crypt_uword_t       *pool,      // IN: the data pool for temps
    size_t               uwords     // IN: number of crypt_uwords in 'pool'
    )
{
    msbnCtx_t            *pBigCtx = (msbnCtx_t *)pool;
    yoyo_allocator       *allocCtx;

    pAssert(pool != NULL && uwords > ALLOCATION_OVERHEAD);

    pool += SIZE_IN_CRYPT_WORDS(msbnCtx_t);
    allocCtx = (yoyo_allocator *)pool;
    pool += SIZE_IN_CRYPT_WORDS(yoyo_allocator);

    // Initialize the msbnCtx_t
    pBigCtx->pAllocCtx = allocCtx;
    pBigCtx->pRandCtx = NULL;
    pBigCtx->latest_errno = MP_ERRNO_NO_ERROR;
    pBigCtx->latest_errno_clue = "";

    // Initialize the yo-yo context
    yoyo_allocator_initialize(allocCtx, pool,
                              RADIX_BYTES * (uwords - ALLOCATION_OVERHEAD));

    NEW_METRIC(name);
    return pBigCtx;
}

void* WINAPI mp_alloc_temp(
    DWORDREGC cb,
    LPCSTR    pszSource_info,
    bigctx_t  *pCtx)
{
    void                *pVoid;
    pMsbnCtx             pBigCtx = (pMsbnCtx)pCtx;

    NOT_REFERENCED(pszSource_info);

    pVoid = yoyo_allocator_alloc(pBigCtx->pAllocCtx, cb);

    if(NULL == pVoid)
    {
        SetMpErrno_clue(MP_ERRNO_NO_MEMORY, "mp_alloc_temp", pCtx);
    }
    else
    {
        memset(pVoid, 0, cb);
    }
    return pVoid;
}

void WINAPI mp_free_temp(
    __in                  void     *pVoid,
    __in_opt              LPCSTR    pszSource_info,
    __in                  bigctx_t  *pCtx)
{
    pMsbnCtx               pBigCtx = (pMsbnCtx)pCtx;
    NOT_REFERENCED(pszSource_info);

    yoyo_allocator_free(pBigCtx->pAllocCtx, pVoid);
}

void WINAPI SetMpErrno(mp_errno_tc code, bigctx_t *pBignumCtxt)
{
    if(NULL != pBignumCtxt)
    {
        pBignumCtxt->latest_errno = code;
    }
}

void WINAPI SetMpErrno_clue1(__in mp_errno_tc code, __in_opt const char *hint,
                             PBIGCTX_ARG)
{
    NOT_REFERENCED(hint);

    SetMpErrno(code, PBIGCTX_PASS);
}

#endif