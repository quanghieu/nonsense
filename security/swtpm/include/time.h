#ifndef _TPM_TIME_H
#define _TPM_TIME_H

#include <linux/time.h>
#include <linux/ktime.h>
#include <linux/types.h>

ktime_t ktime_get(void);
time_t time(time_t *t);
unsigned long long clock(void);

static inline struct tm *
localtime(const time_t *t)
{
    return NULL;
}

static inline struct tm *
gmtime(const time_t *t)
{
    return NULL;
}

#endif
