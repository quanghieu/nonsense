#ifndef _TPM_SETJMP_H
#define _TPM_SETJMP_H

#include <assert.h>

typedef char    jmp_buf;
#define setjmp(x)
#define longjmp(x, y)   assert(0)

#endif
