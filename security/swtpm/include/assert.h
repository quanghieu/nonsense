#ifndef _TPM_ASSERT_H
#define _TPM_ASSERT_H

#include <linux/kernel.h>

// ref: http://kernelnewbies.org/FAQ/BUG
//#define BUUG() __asm__ __volatile__("ud2\n") // invalid instruction
#define BUUG()

#define assert(x)                                                                               \
    do {    if (x) break;                                                                       \
                printk(KERN_EMERG "### ASSERTION FAILED %s: %s: %d: %s\n",                      \
                                       __FILE__, __func__, __LINE__, #x); dump_stack(); BUUG(); \
    } while (0)

#endif
