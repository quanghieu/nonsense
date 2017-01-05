
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <linux/random.h>
#include <linux/slab.h>
#ifdef CONFIG_SWTPM_PROTECTION
#include <linux/data_protection.h>
#endif

#include <openssl/rand.h>
#include <openssl/bn.h>

#ifdef CONFIG_SWTPM_PROTECTION
#define MAX_SHADOW_PAGES    150
typedef struct {
    void *address;
    bool used;
} shadow_page;

shadow_page shadow_pages[MAX_SHADOW_PAGES];

void init_shadow_malloc()
{
    unsigned int i;
    for (i = 0; i < MAX_SHADOW_PAGES; i++) {
        shadow_pages[i].address = NULL;
        shadow_pages[i].used = false;
    }
    for (i = 0; i < MAX_SHADOW_PAGES; i++) {
        while (shadow_pages[i].address == NULL)
            shadow_pages[i].address = kmalloc(PAGE_SIZE, GFP_ATOMIC);
        kdp_protect_one_page_none(shadow_pages[i].address);
    }
}
#endif

char *strerror(int errnum)
{
    return NULL;
}

time_t time(time_t *t)
{
    if(t != NULL) {
        *t = get_seconds();
        return *t;
    }
    return get_seconds();
}

int rand_s(unsigned int *randomValue)
{
    get_random_bytes(randomValue, sizeof(uint32_t));
    return 0;
}

int rand(void)
{
    int tmp;
    rand_s(&tmp);
    return tmp;
}

inline
char *getenv(const char *name)
{
    return NULL;
}

inline
void *BIO_new_file(const char *filename, const char *mode)
{
    return NULL;
}

void ERR_add_error_data_null(int num, ...)
{
    return;
}

void *malloc(size_t size)
{
    void *ptr = NULL;
#ifdef CONFIG_SWTPM_PROTECTION
    unsigned int i;
    if (size <= PAGE_SIZE) {
        for (i = 0; i < MAX_SHADOW_PAGES; i++) {
            if (shadow_pages[i].used)
                continue;
            shadow_pages[i].used = true;
            return shadow_pages[i].address;
        }
    }
    pr_info("[kdp] malloc: no shadow page or page size is too large (%d)\n", size);
#endif
    ptr = kmalloc(size, GFP_ATOMIC);
#ifdef CONFIG_SWTPM_PROTECTION
    kdp_protect_one_page_none(ptr);
#endif
    return ptr;
}

void free(void *ptr)
{
#ifdef CONFIG_SWTPM_PROTECTION
    unsigned int i;
    for (i = 0; i < MAX_SHADOW_PAGES; i++) {
        if (shadow_pages[i].used && shadow_pages[i].address == ptr) {
            shadow_pages[i].used = false;
            return;
        }
    }
    // unprotect page;
    pr_info("[kdp] free: kdp_unprotect_one_page\n");
    kdp_unprotect_one_page(ptr);
#endif
    kfree(ptr);
}

void *realloc(void *ptr, size_t size)
{
    void *new_ptr = kmalloc(size, GFP_ATOMIC);
    pr_info("[kdp] realloc: ptr = 0x%p, size = %lu", ptr, size);

    memcpy(new_ptr, ptr, size);
    kfree(ptr);

    return new_ptr;
}

/* stdio.h */
int BIO_printf(void *bio, const char *format, ...)
{
    va_list ap;
    int r;

    va_start(ap, format);
    r = vprintk(format, ap);
    va_end(ap);
    return r;
}

int BIO_snprintf(char *str, size_t size, const char *format, ...)
{
    va_list ap;
    int r;

    va_start(ap, format);
    r = vsnprintf(str, size, format, ap);
    va_end(ap);
    return r;
}

int
RAND_poll(void)
{
    RAND_add("TEST", 4, 4);
    return 0;
}

unsigned long long clock(void)
{
    return ktime_to_ms(ktime_get());
}

