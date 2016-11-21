/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Description
// This file will instance the TPM variables that are not stack allocated. The
// descriptions for these variables are in Global.h for this project.

//** Includes
#include    "Implementation.h"
#include    "PlatformData.h"

// From Cancel.c
BOOL                 s_isCanceled;

// From Clock.c
unsigned int         s_adjustRate;
BOOL                 s_timerReset;
BOOL                 s_timerStopped;

#ifndef HARDWARE_CLOCK
#include    <time.h>
clock_t             s_realTimePrevious;
clock_t             s_tpmTime;
#endif


// From LocalityPlat.c
unsigned char        s_locality;

// From Power.c
BOOL                 s_powerLost;

// From Entropy.c
uint32_t             lastEntropy;
int                  firstValue;

// From NVMem.c
#ifdef  VTPM
#   undef FILE_BACKED_NV
#endif
#ifdef FILE_BACKED_NV
FILE                *s_NVFile = NULL;
#endif
unsigned char        s_NV[NV_MEMORY_SIZE];
BOOL                 s_NvIsAvailable;
BOOL                 s_NV_unrecoverable;
BOOL                 s_NV_recoverable;

// From PPPlat.c
BOOL  s_physicalPresence;