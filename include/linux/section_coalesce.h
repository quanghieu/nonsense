#ifndef LINUX_SECTION_COALESCE_H
#define LINUX_SECTION_COALESCE_H

#include <linux/mm_types.h>
#include <linux/types.h>

#define SC_SIZE     ((0x003F0000 - 0x10000) / 2)
#define SC_COUNT    (SC_SIZE / PAGE_SIZE)
#define SC_BASE     0x40010000


#ifdef CONFIG_SECTION_COALESCE

extern char sc_status[SC_COUNT];
int __init sc_init(void);
void __init *sc_early_alloc(unsigned long sz);
unsigned long sc_get_free_page(void);
unsigned long sc_free_page(unsigned long addr);

#else

#define sc_init()
#define sc_early_alloc(sz)  NULL
#define sc_get_free_page()  0
#define sc_free_page(addr)  addr

#endif  /* CONFIG_SECTION_COALESCE */

#endif
