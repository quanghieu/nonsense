#include <linux/mm.h>
#include <linux/memblock.h>
#include <linux/printk.h>
#include <linux/section_coalesce.h>
#include <asm/proc-fns.h>

#define PG_DIR_SIZE 0x4000

int __init sc_init(void)
{
    pr_info("[sc] sc_init start\n");
    memset(sc_status, 0, 0x003F0000 - (2 * PG_DIR_SIZE));    // shadow_pg_dir, swapper_pg_dir
    return 0;
}

phys_addr_t sc_memblock_alloc(phys_addr_t size)
{
    int i;

    if (size != PAGE_SIZE) {
        pr_warn("[sc] sc_membloc_alloc: the size is not aligned (size: 0x%x)\n", size);
        return 0;
    }

    for (i = 0; i < SC_COUNT; i++) {
        if (sc_status[i] == 0) {
            sc_status[i] = 1;
            return (phys_addr_t)(SC_BASE + i * PAGE_SIZE);
        }
    }

    pr_warn("[sc] sc_memblock_alloc: there are no free space\n");
    return 0;
}

void __init *sc_early_alloc(unsigned long sz)
{
    phys_addr_t phys_addr = sc_memblock_alloc(sz);
    void *ptr = NULL;

    if (phys_addr != 0) {
        ptr = __va(phys_addr);
        memset(ptr, 0, sz);
    }

    return ptr;
}

unsigned long sc_get_free_page(void)
{
    return (unsigned long) __va(sc_memblock_alloc(PAGE_SIZE));
}

unsigned long sc_free_page(unsigned long addr)
{
    pr_info("[sc] sc_free_page: addr = 0x%lx\n", addr);
    return addr;
}
