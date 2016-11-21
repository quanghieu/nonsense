/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include <memory.h>

#include "Implementation.h"
#include "LibSupport.h"

#if MATH_LIB == MSBN

#include "msbn/YoYoAllocator.h"

#define ADVANCE_VOID_PTR(__PTR__, __CNT__)  (void*)((size_t)__PTR__ + __CNT__)
#define RETREAT_VOID_PTR(__PTR__, __CNT__)  (void*)((size_t)__PTR__ - __CNT__)

void insert_tail_list(
    list_entry      *ptail,
    list_entry      *pentry
    )
{
    pentry->pprev = ptail->pprev;
    pentry->pprev->pnext = pentry;

    pentry->pnext = ptail;
    ptail->pprev = pentry;
}

void remove_list_entry(
    list_entry  *pentry)
{
    pentry->pnext->pprev = pentry->pprev;
    pentry->pprev->pnext = pentry->pnext;
}

// initialize_simple_allocator
// with a supplied buffer
void yoyo_allocator_initialize(
    yoyo_allocator  *pallocator,
    void            *pv_buffer,
    size_t           cnt_buffer
    )
{
    memset((void*)pallocator, 0, sizeof(yoyo_allocator));

    pallocator->pv_buffer = pv_buffer;
    pallocator->cnt_buffer = cnt_buffer;
    pallocator->pv_current = pallocator->pv_buffer;

    pallocator->head.pprev = NULL;
    pallocator->head.pnext = &(pallocator->tail);
    pallocator->tail.pprev = &(pallocator->head);
    pallocator->tail.pnext = NULL;
}

void* yoyo_allocator_alloc(
    yoyo_allocator  *pallocator,
    size_t           cnt_alloc
    )
{
    void*   pv = NULL;

    size_t  cnt_next_allocated;
    size_t  cnt_this_alloc;

    yoyo_allocation_header  *pallochdr = NULL;
    list_entry  *pentry = NULL;

    cnt_this_alloc = cnt_alloc + sizeof(yoyo_allocation_header);

    cnt_next_allocated = pallocator->cnt_allocated + cnt_this_alloc;

    if(pallocator->cnt_buffer < cnt_next_allocated)
    {
        goto exit;
    }

    pv = pallocator->pv_current;
    pallocator->pv_current = ADVANCE_VOID_PTR(pallocator->pv_current,
                                              cnt_this_alloc);
    pallocator->cnt_allocated = cnt_next_allocated;

    pentry = (list_entry*)pv;
    pallochdr = (yoyo_allocation_header*)pv;
    pv = ADVANCE_VOID_PTR(pv, sizeof(yoyo_allocation_header));

    insert_tail_list(&(pallocator->tail), pentry);

    pallochdr->cnt_alloc = cnt_this_alloc;

    pallocator->count_allocations++;

    if(pallocator->max_count_allocations < pallocator->count_allocations)
    {
        pallocator->max_count_allocations = pallocator->count_allocations;
    }

    if(pallocator->max_cnt_allocated < pallocator->cnt_allocated)
    {
        pallocator->max_cnt_allocated = pallocator->cnt_allocated;
    }

exit:
    return pv;
}

void yoyo_allocator_free(
    yoyo_allocator  *pallocator,
    void            *pv_to_free
    )
{
    void*   pv = NULL;

    list_entry* pentry = NULL;

    if((pallocator->pv_buffer <= pv_to_free) &&
       ((size_t)pv_to_free < ((size_t)pallocator->pv_buffer
                              + pallocator->cnt_allocated)))
    {
        pallocator->count_allocations--;
    }

    pv = RETREAT_VOID_PTR(pv_to_free, sizeof(yoyo_allocation_header));

    pentry = (list_entry*)pv;

    if(pentry == pallocator->tail.pprev)
    {
        if((&pallocator->head != pentry->pprev))
        {
            void*   pv_new_end = (void*)pentry->pprev;
            yoyo_allocation_header* pallochdr = (yoyo_allocation_header*)pv_new_end;
            pallocator->pv_current = ADVANCE_VOID_PTR(pv_new_end,
                                                      pallochdr->cnt_alloc);
        }
        else
        {
            pallocator->pv_current = pallocator->pv_buffer;
        }
        pallocator->cnt_allocated = (size_t)pallocator->pv_current
            - (size_t)pallocator->pv_buffer;
    }
    remove_list_entry(pentry);
}

void yoyo_allocator_uninitialize(
    yoyo_allocator  *pallocator
    )
{
    memset((void*)pallocator, 0, sizeof(yoyo_allocator));
}

#endif // MATH_LIB == MSBN