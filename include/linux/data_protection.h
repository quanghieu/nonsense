#ifndef LINUX_DATA_PROTECTION_H
#define LINUX_DATA_PROTECTION_H

#include <linux/mm_types.h>
#include <linux/types.h>
#include <linux/section_coalesce.h>
#include <asm/proc-fns.h>

#ifndef PG_DIR_SIZE
#ifdef CONFIG_ARM_LPAE
#define PG_DIR_SIZE	0x5000  /* LPAE requires an additional page for the PGD */
#else
#define PG_DIR_SIZE	0x4000
#endif
#endif

#define SHADOW_DIR_SIZE     PG_DIR_SIZE
#define SHADOW_OFFSET       SC_SIZE


#ifdef CONFIG_DATA_PROTECTION

extern int kdp_enabled;

void kdp_enable(void);
void kdp_protect_one_page(void *p);
void kdp_protect_one_page_none(void *p);
void kdp_unprotect_one_page(void *p);
void protect_module(void *base,
        unsigned long text_size,
        unsigned long ro_size,
        unsigned long total_size);
void shadow_pmd_populate(pmd_t *pmdp, pmdval_t pmdval);
void shadow_set_pte_ext(pte_t *ptep, pte_t pte, unsigned long ext);

// switch gate
unsigned long shadow_entry_gate(void);
void shadow_exit_gate(unsigned long);
#define entry_gate()    unsigned long shadow_flags = shadow_entry_gate();
#define exit_gate()     shadow_exit_gate(shadow_flags);

#else

#define kdp_enabled 0

#define kdp_enable()

#define entry_gate()
#define exit_gate()
#define shadow_pmd_populate(pmdp, pmdval)
#define kdp_protect_one_page(p)
#define kdp_protect_one_page_none(p)
#define kdp_unprotect_one_page(p)

#endif  /* CONFIG_DATA_PROTECTION */

#endif  /* LINUX_DATA_PROTECTION_H */
