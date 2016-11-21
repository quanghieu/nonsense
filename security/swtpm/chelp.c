
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <linux/random.h>
#include <linux/slab.h>

#include <openssl/rand.h>
#include <openssl/bn.h>

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
    ptr = kmalloc(size, GFP_ATOMIC);
    return ptr;
}

void free(void *ptr)
{
    kfree(ptr);
}

void *realloc(void *ptr, size_t size)
{
    void *new_ptr = kmalloc(size, GFP_ATOMIC);
    LogInfo("realloc: ptr = 0x%p, size = %u", ptr, size);

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

