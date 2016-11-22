#include <linux/mm.h>
#include <linux/memblock.h>
#include <linux/printk.h>
#include <linux/data_protection.h>
#include <asm/proc-fns.h>
#include <asm/irqflags.h>

#define TEXT_ALIGN __attribute__ ((aligned(4096)))
#define KDP_CODE __attribute__ ((section ("kdp_text")))
#define SWITCH_GATE __attribute__ ((section ("switch_gate")))

#define OPTIMIZE __attribute__((optimize("O0")))

int kdp_enabled;
EXPORT_SYMBOL(kdp_enabled);

enum pg_level {
    PG_LEVEL_NONE,
    PG_LEVEL_4K,
    PG_LEVEL_2M,
    PG_LEVEL_1G,
    PG_LEVEL_NUM
};

static void OPTIMIZE walk_pte(pmd_t *pmd, pmd_t *shadow_pmd, unsigned long start)
{
	pte_t *pte = pte_offset_kernel(pmd, 0);
    pte_t *shadow_pte = pte_offset_kernel(shadow_pmd, 0);
	unsigned i;

	for (i = 0; i < PTRS_PER_PTE * 2; i++, pte++, shadow_pte++) {
        *shadow_pte = pte_val(*pte);
	}
}

static void OPTIMIZE walk_pmd(pud_t *pud, pud_t *shadow_pud, unsigned long start)
{
	pmd_t *pmd = pmd_offset(pud, 0);
    pmd_t *shadow_pmd = pmd_offset(shadow_pud, 0);
	unsigned long addr;
	unsigned i;

	for (i = 0; i < PTRS_PER_PMD; i++, pmd++, shadow_pmd++) {
		addr = start + i * PMD_SIZE;

        if (pmd_none(*pmd)) {
            // do nothing
            continue;
        } else if (pmd_large(*pmd) || !pmd_present(*pmd)) {
            // section
            shadow_pmd[0] = __pmd(pmd_val(*pmd));
#ifndef CONFIG_ARM_LPAE
            shadow_pmd[1] = __pmd(pmd_val(pmd[1]));
#endif
        } else {
            // page table
            if (0 && addr >= 0xf0200000 && addr < 0xf0700000) {
                shadow_pmd[0] = __pmd(pmd_val(pmd[0]));
#ifndef CONFIG_ARM_LPAE
                shadow_pmd[1] = __pmd(pmd_val(pmd[1]));
#endif

            } else {
                shadow_pmd[0] = __pmd(pmd_val(pmd[0]) + SHADOW_OFFSET);
#ifndef CONFIG_ARM_LPAE
                shadow_pmd[1] = __pmd(pmd_val(pmd[1]) + SHADOW_OFFSET);
#endif
                walk_pte(pmd, shadow_pmd, addr);
            }
        }
	}
}

static void OPTIMIZE walk_pud(pgd_t *pgd, pgd_t *shadow_pgd, unsigned long start)
{
	pud_t *pud = pud_offset(pgd, 0);
    pud_t *shadow_pud = pud_offset(shadow_pgd, 0);
	unsigned long addr;
	unsigned i;

	for (i = 0; i < PTRS_PER_PUD; i++, pud++, shadow_pud++) {
		addr = start + i * PUD_SIZE;
		if (!pud_none(*pud)) {
			walk_pmd(pud, shadow_pud, addr);
		} else {
            pr_err("[kdp]   pud_none\n");
		}
	}
}

void OPTIMIZE walk_pgd(void)
{
	pgd_t *pgd = swapper_pg_dir;
    pgd_t *shadow_pgd = shadow_pg_dir;
	unsigned long addr;
	unsigned i;

	for (i = 0; i < PTRS_PER_PGD; i++, pgd++, shadow_pgd++) {
		addr = i * PGDIR_SIZE;
		if (!pgd_none(*pgd)) {
			walk_pud(pgd, shadow_pgd, addr);
		} else {
            pr_err("[kdp] pgd_none\n");
		}
	}
}

int OPTIMIZE __init kdp_init(void)
{
    printk("[kdp] kdp_init start\n");
    memset(shadow_pg_dir, 0, SHADOW_DIR_SIZE);
    return 0;
}
early_initcall(kdp_init);


static pte_t* OPTIMIZE lookup_address(unsigned long address, unsigned int *level)
{
    pgd_t *pgd = NULL;
    pud_t *pud = NULL;
    pmd_t *pmd = NULL;
    pte_t *pte = NULL;

    *level = PG_LEVEL_NONE;

//    pgd = pgd_offset_k(address);
    pgd = (pgd_t *)pgd_offset_s(address);
    pud = pud_offset(pgd, address);
    if (unlikely(pud_none(*pud)))
        return NULL;

    *level = PG_LEVEL_1G;
    /* This sould never happen */
    if (unlikely(pud_bad(*pud)))
        return (pte_t *)pud;

    pmd = pmd_offset(pud, address);
    if (unlikely(pmd_none(*pmd)))
        return NULL;

    *level = PG_LEVEL_2M;
    /* This should never happen in page granularity */
    /* Section region (in 32-bit, 1MB memory region) */
    if (unlikely(pmd_bad(*pmd)))
        return (pte_t *)pmd;

    *level = PG_LEVEL_4K;
    pte = pte_offset_kernel(pmd, address);

    return pte;
}

static void inline flush_kern_tlb_one_page(void *address)
{
    asm (
    "   dsb ishst\n"
    "   lsr %0, %0, #12\n"
    "   mcr p15, 0, %0, c8, c3, 3\n"
    "   dsb ish\n"
    "   isb"
    : : "r" (address));
}

static void inline kdp_flush_pte(void *address)
{
    asm (
    "   mcr p15, 0, %0, c7, c10, 1"
    : : "r" (address));
}

static void OPTIMIZE _kdp_protect_one_page(void *address, pgprot_t prot)
{
    pte_t *ptep, pte;
    unsigned int level;

    if (unlikely(address == NULL))
        return;

    ptep = lookup_address((unsigned long)address, &level);
    BUG_ON(!ptep);
    // currently, we handle section or page granularity
    if ((level != PG_LEVEL_4K) && (level != PG_LEVEL_2M))
        return;

    if (level == PG_LEVEL_2M) {
        // Section memory region (1M)
        pte = (pte_t)pmd_modify(*ptep, prot);
        if (unlikely(pte == *ptep))
            return;
    } else if (level == PG_LEVEL_4K) {
        // Page memory region (4K)
        pte = (pte_t)pte_modify(*ptep, prot);
        if (unlikely(pte == *ptep))
            return;
    }

    if (likely(kdp_enabled)) {
        unsigned long flags = shadow_entry_gate();
        shadow_set_pte_ext(ptep, pte, 0);
        shadow_exit_gate(flags);
        flush_kern_tlb_one_page(address);
    } else {
        shadow_set_pte_ext(ptep, pte, 0);
        flush_kern_tlb_one_page(address);
    }
}

void OPTIMIZE kdp_protect_one_page(void *address)
{
    _kdp_protect_one_page(address, PAGE_KERNEL_READONLY);
}
EXPORT_SYMBOL(kdp_protect_one_page);

void kdp_protect_one_page_none(void *address)
{
    _kdp_protect_one_page(address, PAGE_KERNEL_NONE);
}
EXPORT_SYMBOL(kdp_protect_one_page_none);

void kdp_unprotect_one_page(void *address)
{
    pte_t *ptep, pte; 
    unsigned int level;

    if (unlikely(address == NULL))
        return;

    if (unlikely(!kdp_enabled)) {
        pr_err("KDFI not enabled\n");
        return;
    }

    ptep = lookup_address((unsigned long)address, &level);
    BUG_ON(!ptep);
    BUG_ON(level != PG_LEVEL_4K);

    pte = pte_modify(*ptep, PAGE_KERNEL);
    kdp_set_pte_ext(ptep, pte, 0);
    flush_kern_tlb_one_page(address);
}
EXPORT_SYMBOL(kdp_unprotect_one_page);

void OPTIMIZE kdp_protect_one_section(void *address)
{
    _kdp_protect_one_page(address, SECT_KERNEL_READONLY);    // SECT_KERNEL_READONLY_PMD
}

static void protect_pgtable(pgd_t *pg_dir)
{
    /*
     * traverse the whole page table and
     * make every page translation struct as read-only
     * under direct mapping
     */
    pgd_t *pgd = pg_dir;
    pud_t *pud = NULL;
    pmd_t *pmd = NULL;
    pte_t *pte = NULL;
    unsigned i, j, k;

    /* first, protect the page directory page */
    kdp_protect_one_page(pg_dir + (0x3000/8));  // e.g., swapper_pg_dir = 0xC0004000, kernel mapping in0xC0007000

    /* walk_pgd */
	for (i = 0; i < PTRS_PER_PGD; i++, pgd++) {
        if (pgd_none(*pgd))
            continue;

        /* walk_pud */
        pud = pud_offset(pgd, 0);
        for (j = 0; j < PTRS_PER_PUD; j++, pud++) {
            if (pud_none(*pud))
                continue;

            /* walk_pmd */
            pmd = pmd_offset(pud, 0);
            for (k = 0; k < PTRS_PER_PMD; k++, pmd++) {
                /* skip sections as well */
                if (pmd_none(*pmd) || pmd_large(*pmd) || !pmd_present(*pmd))
                    continue;

                /* finally, protect valid pte pages */
                pr_info("we need to protect the pte page by pmd entry: 0x%p(= 0x%x)\n", pmd, pmd_val(*pmd));
                pr_info(" - pte_offset_kernel(pmd, 0) = 0x%p\n", pte_offset_kernel(pmd, 0));
                pte = pte_offset_kernel(pmd, 0);
                kdp_protect_one_page(pte);
            }
        }
    }
}

void OPTIMIZE kdp_enable(void)
{
    unsigned long attr = 0;
    unsigned long flags;

    attr = cpu_get_ttbr(0) & 0x3fff;
    cpu_set_ttbr(1, virt_to_phys(shadow_pg_dir) | attr);
    pr_info("ttbr0: 0x%lx, ttbr1: 0x%lx\n", cpu_get_ttbr(0), cpu_get_ttbr(1));
    pr_info("swapper_pg_dir: 0x%p, phys: 0x%x\n", swapper_pg_dir, virt_to_phys(swapper_pg_dir));
    pr_info("shadow_pg_dir: 0x%p, phys: 0x%x\n", shadow_pg_dir, virt_to_phys(shadow_pg_dir));

    /*
     * try to map all physical memory banks
     */
    walk_pgd();
    // TODO: protect_kernel();

    /*
     * enable shadow page table, this is necessary for
     * making swapper_pg_dir as read-only
     */
    flags = shadow_entry_gate();

    /* protect all init page tables */
    protect_pgtable(swapper_pg_dir);
    protect_pgtable(shadow_pg_dir);

    /* set data protection as enabled */
    kdp_enabled = 1;

    /* restore old pgd */
    shadow_exit_gate(flags);
    pr_info("[kdp] kdp enabled\n");
    flush_tlb_all();
}


// only run with shadow space
void KDP_CODE TEXT_ALIGN kdp_text_start(void) {}

void KDP_CODE shadow_pmd_populate(pmd_t *pmdp, pmdval_t pmdval)
{
    pmdp[0] = __pmd(pmdval);
#ifndef CONFIG_ARM_LPAE
    pmdp[1] = __pmd(pmdval + 256 * sizeof(pte_t));
#endif
}

void KDP_CODE shadow_set_pte_ext(pte_t *ptep, pteval_t pte, unsigned long ext)
{
    pte_t *hw_ptep = ptep + (2048/sizeof(pte_t));
    pteval_t hw_pte = 0x0;

    *ptep = __pte(pte); // linux version

    hw_pte = pte & ~(0x000003f0);
    hw_pte = hw_pte & ~(PTE_TYPE_MASK);
    hw_pte |= ext;
    hw_pte |= PTE_EXT_AP0 | 2;

    if (hw_pte & (1 << 4))
        hw_pte |= PTE_EXT_TEX(1);

    pte ^= L_PTE_DIRTY; // exclusive or

    if (pte & (L_PTE_RDONLY | L_PTE_DIRTY))
        hw_pte |= PTE_EXT_APX;

    if (pte & L_PTE_USER)
        hw_pte |= PTE_EXT_AP1;

    if (pte & L_PTE_XN)
        hw_pte |= PTE_EXT_XN; 

    if (!(pte & L_PTE_YOUNG) || !(pte & L_PTE_VALID) || (pte & L_PTE_NONE))
        hw_pte = 0;

    *hw_ptep = __pte(hw_pte);
    kdp_flush_pte(hw_ptep);
}

void KDP_CODE kdp_text_end(void) {}


void SWITCH_GATE TEXT_ALIGN switch_gate_start(void) {}

unsigned long SWITCH_GATE shadow_entry_gate(void)
{
    unsigned long flags;
    unsigned long ttbcr = 0x11;

    asm volatile(
            "   isb\n"
            "   mrs     %0, " IRQMASK_REG_NAME_R "  @ local_save_flags\n"
            "   cpsid   i       @ arch_local_irq_disable\n"
            "   cpsid   f       @ __clf\n"
            "   isb"
            : "=r" (flags)
            :   
            : "memory", "cc");
    asm volatile(
            "   isb\n"
            "   mcr     p15, 0, %0, c2, c0, 2\n"
            "   isb\n"
            "   mcr     p15, 0, %0, c8, c7, 0\n"
            "   mcr     p15, 0, %0, c8, c6, 0\n"
            "   mcr     p15, 0, %0, c8, c5, 0\n"
            "   dsb\n"
            "   isb"
            :   
            : "r" (ttbcr)
            : "cc");

    return flags;
}
EXPORT_SYMBOL(shadow_entry_gate);

void SWITCH_GATE shadow_exit_gate(unsigned long flags)
{
    unsigned long ttbcr = 0x0;
    asm volatile(
            "   isb\n"
            "   mcr     p15, 0, %1, c2, c0, 2\n"
            "   isb\n"
            "   mcr     p15, 0, %1, c8, c7, 0\n"
            "   mcr     p15, 0, %1, c8, c6, 0\n"
            "   mcr     p15, 0, %1, c8, c5, 0\n"
            "   dsb\n"
            "   isb\n"
            "   msr " IRQMASK_REG_NAME_W ", %0  @ local_irq_restore\b"
            "   isb"
            :
            : "r" (flags), "r" (ttbcr)
            : "cc");
}
EXPORT_SYMBOL(shadow_exit_gate);

void SWITCH_GATE switch_gate_end(void) {}

