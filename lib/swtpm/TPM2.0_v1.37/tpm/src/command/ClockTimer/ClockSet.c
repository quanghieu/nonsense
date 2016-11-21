/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include "Tpm.h"
#include "ClockSet_fp.h"

#ifdef TPM_CC_ClockSet  // Conditional expansion of this file

// Read the current TPMS_TIMER_INFO structure settings
// return type: TPM_RC
//   TPM_RC_NV_RATE              NV is unavailable because of rate limit
//   TPM_RC_NV_UNAVAILABLE       NV is inaccessible
//   TPM_RC_VALUE                invalid new clock

TPM_RC
TPM2_ClockSet(
    ClockSet_In     *in             // IN: input parameter list
    )
{
#define CLOCK_UPDATE_MASK  ~((1ULL << NV_CLOCK_UPDATE_INTERVAL)- 1)

// Input Validation

    // new time can not be bigger than 0xFFFF000000000000 or smaller than
    // current clock
    if(in->newTime > 0xFFFF000000000000ULL
       || in->newTime < go.clock)
        return TPM_RCS_VALUE + RC_ClockSet_newTime;

// Internal Data Update

    return TimeClockUpdate(in->newTime);
}

#endif // CC_ClockSet