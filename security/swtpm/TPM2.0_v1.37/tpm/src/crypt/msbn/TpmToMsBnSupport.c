/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
//
// The functions in this file are used for initialization of the interface to the
// MsBn library

//** Defines and Includes

#include "Tpm.h"

#if MATH_LIB == MSBN

//*** SupportLibInit()
// This does any initialization required by the support library.
LIB_EXPORT int
SupportLibInit(
    void
    )
{
    return 1;
}

#ifdef COLLECT_METRICS
_REDUCE_WARNING_LEVEL_(2)
#include <stdio.h>
_NORMAL_WARNING_LEVEL_
extern memoryMetric memoryMetrics[];
#endif
//*** MathLibSimulationEnd()
// Function to print statistics
void
MathLibSimulationEnd(
    void
    )
{
#ifdef COLLECT_METRICS
    int         i;
    printf("Memory usage statistics:\nAllocation overhead: %d bytes\n",
           ALLOCATION_OVERHEAD * RADIX_BYTES);
    printf("Function            Bytes Used\n");
    for(i = 0; i < MAX_METRICS; i++)
    {
        if(memoryMetrics[i].name != NULL)
            printf("%-20s%d\n", memoryMetrics[i].name,
                   (int)memoryMetrics[i].highWater);
    }
#endif
}

//*** random_digits()
// Function called by MsBn to get random digits.
LIB_EXPORT BOOL WINAPI
random_bytes(
    BYTE            *result,
    size_t           count,
    bigctx_t        *ctx
    )
{
#if 0
    DRBG_Generate((RAND_STATE *)((pMsbnCtx)ctx->prandctx), result,
                  (UINT16)count);
    return TRUE;
#else
    // no current reason for MsBn to call for random numbers.
    (result);
    (count);
    (ctx);
    FAIL(FATAL_ERROR_INTERNAL);
#endif
}

#endif