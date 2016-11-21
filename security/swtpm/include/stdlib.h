#ifndef _TPM_STDLIB_H
#define _TPM_STDLIB_H

#include <assert.h>
#include <linux/string.h>
#include <linux/sort.h>
#include <linux/kernel.h>

#ifndef FILE
typedef void * FILE;
#endif

#define abort() assert(0)
#define qsort(base, nmemb, size, compar) sort(base, nmemb, size, compar, NULL)

#define strtol  simple_strtol
#define strtoul simple_strtoul

int rand_s(unsigned int *randomValue);
int rand(void);
char *getenv(const char *name);

void *malloc(size_t size);
void free(void *ptr);
void *realloc(void *ptr, size_t size);

#endif
