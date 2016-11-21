#ifndef _TPM_STDIO_H
#define _TPM_STDIO_H

#include <linux/types.h>
#include <linux/kernel.h>
//#include <stdarg.h>

#define EOF (-1)
#define stdin NULL

#ifndef FILE
typedef void * FILE;
#endif
//typedef unsigned long size_t;

static inline char *
fgets(char *s, int size, FILE *fp)
{
    return NULL;
}

static inline int
fputs(const char *s, FILE *fp)
{
    return EOF;
}

static inline int
fflush(FILE *s)
{
    return EOF;
}

static inline int
fprintf(FILE *fp, const char *format, ...)
{
    return -1;
}

static inline int
feof(FILE *fp)
{
    return -1;
}

static inline int
ferror(FILE *fp)
{
    return -1;
}

static inline FILE *
fopen(const char *path, const char *mode)
{
    return NULL;
}

static inline int 
fileno(FILE *fp)
{
    return -1;
}

static inline int 
fclose(FILE *fp)
{
    return EOF;
}

int
sscanf(const char *str, const char *format, ...);

static inline size_t
fwrite(const void *buf, size_t size, size_t num, FILE *fp)
{
    return 0;
}

#endif
