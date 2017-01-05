/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

// This file contains the instance data for the Platform module. It is collected
// in this file so that the state of the module is easier to manage.

#ifndef _PLATFORM_DATA_H_
#define _PLATFORM_DATA_H_


#include      "Implementation.h"

// From Cancel.c
// Cancel flag.  It is initialized as FALSE, which indicate the command is not
// being canceled
extern int     s_isCanceled;

#include    <time.h>
typedef struct {
        time_t tv_sec;  // Seconds - >= 0
        long   tv_nsec; // Nanoseconds - [0, 999999999]
} timespec_t;

#ifndef HARDWARE_CLOCK
// This is the value returned the last time that the system clock was read. This
// is only relevant for a simulator or virtual TPM.
extern clock_t        s_realTimePrevious;
// This is the rate adjusted value that is the equivalent of what would be read from
// a hardware register that produced rate adjusted time.
extern clock_t        s_tpmTime;
#endif // HARDWARE_CLOCK

// This value indicates that the timer was reset
extern BOOL              s_timerReset;
// This value indicates that the timer was stopped. It causes a clock discontinuity.
extern BOOL              s_timerStopped;

// Assume that the nominal divisor is 30000
#define     CLOCK_NOMINAL           30000
// A 1% change in rate is 300 counts
#define     CLOCK_ADJUST_COARSE     300
// A .1 change in rate is 30 counts
#define     CLOCK_ADJUST_MEDIUM     30
// A minimum change in rate is 1 count
#define     CLOCK_ADJUST_FINE       1
// The clock tolerance is +/-15% (4500 counts)
// Allow some guard band (16.7%)
#define     CLOCK_ADJUST_LIMIT      5000

// This variable records the time when _plat__TimerReset is called.  This mechanism
// allow us to subtract the time when TPM is power off from the total
// time reported by clock() function
extern uint64_t        s_initClock;

// This variable records the timer adjustment factor.
extern unsigned int         s_adjustRate;

// From LocalityPlat.c
// Locality of current command
extern unsigned char s_locality;

// From NVMem.c
// Choose if the NV memory should be backed by RAM or by file.
// If this macro is defined, then a file is used as NV.  If it is not defined,
// then RAM is used to back NV memory. Comment out to use RAM.
#ifndef NO_FILE_BACKED_NV
#define FILE_BACKED_NV
#endif
#if defined FILE_BACKED_NV
#include <stdio.h>
// A file to emulate NV storage
extern FILE*             s_NVFile;
#endif
extern unsigned char     s_NV[NV_MEMORY_SIZE];
extern BOOL              s_NvIsAvailable;
extern BOOL              s_NV_unrecoverable;
extern BOOL              s_NV_recoverable;
extern BOOL              s_NvAtomicIsPrepared;

// To support atomic write
typedef struct {
    uint32_t    atomic;
    uint32_t    encrypt;
    uint32_t    localVersion;
    uint32_t    commitVersion;
} rpmb_atomic_t;
extern rpmb_atomic_t     s_NvAtomic[NV_MEMORY_SIZE/256];


// From PPPlat.c
// Physical presence.  It is initialized to FALSE
extern BOOL     s_physicalPresence;

// From Power
extern BOOL        s_powerLost;

// From Entropy.c
extern uint32_t        lastEntropy;

extern int             firstValue;

extern BOOL            s_moduleInit;

#endif // _PLATFORM_DATA_H_
