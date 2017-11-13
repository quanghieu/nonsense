/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
// This file contains the routines that are used by the simulator to mimic
// a hardware clock on a TPM.

// In this implementation, all the time values are measured in millisecond.
// However, the precision of the clock functions may be implementation dependent.

//** Includes and Data Definitions
#include "PlatformData.h"
#include "Platform_fp.h"
#include "TpmFail_fp.h"
#include "Tpm.h"
#include <assert.h>
#ifdef HELPER
#include <helper.h>
#endif

//** Simulator Functions
//*** Introduction
// This set of functions is intended to be called by the simulator environment in
// order to simulate hardware events.

//***_plat__TimerReset()
// This function sets current system clock time as t0 for counting TPM time.
// This function is called at a power on event to reset the clock.
LIB_EXPORT void
_plat__TimerReset(
    void
    )
{
    s_realTimePrevious = clock();
    s_tpmTime = 0;
    s_adjustRate = CLOCK_NOMINAL;
    s_timerReset = TRUE;
    s_timerStopped = TRUE;
    return;
}

//*** _plat__TimerRestart()
// This function should be called in order to simulate the restart of the timer
// should it be stopped while power is still applied.
LIB_EXPORT void
_plat__TimerRestart(
    void
    )
{
    s_timerStopped = TRUE;
    return;
}


//** Functions Used by TPM
//*** Introduction
// These functions are called by the TPM code. They should be replaced by
// appropriated hardware functions.

//***_plat__TimerRead()
// This function provides access to the tick timer of the platform. The TPM code 
// uses this value to drive the TPM Clock.
//
// The tick timer is supposed to run when power is applied to the device. This timer
// should not be reset by time events including _TPM_Init. It should only be reset
// when TPM power is re-applied.
//
// If the TPM is run in a protected environment, that environment may provide the
// tick time to the TPM as long as the time provided by the environment is not
// allowed to go backwards. If the time provided by the system can go backwards
// during a power discontinuity, then the _plat__Signal_PowerOn should call
// _plat__TimerReset().
//
// The code in this function should be replaced by a read of a hardware tick timer.
LIB_EXPORT uint64_t
_plat__TimerRead(
    void
    )
{
#ifdef HARDWARE_CLOCK
#error      "need a defintion for reading the hardware clock"
    return HARDWARE_CLOCK
#else
#define BILLION     1000000000
#define MILLION     1000000
#define THOUSAND    1000  
    clock_t         timeDiff;
    uint64_t        adjusted;
#ifdef _ARM_
    uint64_t        tmp;
#endif

#ifdef HELPER
    if (s_moduleInit) {
        if (go.cloudClock == 0) {
//            go.cloudClock = _cloud__TimerRead();
            LogDebug("Update cloud clock! cloud(%llu), local(%llu)", go.cloudClock, clock());
        }
    }
#endif

#   define  TOP     (THOUSAND * CLOCK_NOMINAL)
#   define  BOTTOM  ((uint64_t)s_adjustRate * CLOCKS_PER_SEC)

    // Save the value previously read from the system clock
    timeDiff = s_realTimePrevious;
    // update with the current value of the system clock
    s_realTimePrevious = clock();
    // In the place below when we "put back" the unused part of the timeDiff
    // it is possible that we can put back more than we take out. That is, we could
    // take out 1000 mSec, rate adjust it and put back 1001 mS. This means that
    // on a subsequent call, time may not have caught up. Rather than trying
    // to rate adjust this, just stop time. This only occurs in a simulation so 
    // time for more than one command being the same should not be an issue.
    if(timeDiff >= s_realTimePrevious)
    {
        s_realTimePrevious = timeDiff;
        return s_tpmTime;
    }
    // Compute the amount of time since the last call to the system clock
    timeDiff = s_realTimePrevious - timeDiff;

    // Do the time rate adjustment and conversion from CLOCKS_PER_SEC to mSec
#ifdef _ARM_
    tmp = ((uint64_t)timeDiff * TOP);
    do_div(tmp, BOTTOM);
    adjusted = tmp;
#else
    adjusted = (((uint64_t)timeDiff * TOP) / BOTTOM);
#endif

    s_tpmTime += (clock_t)adjusted;

    // Might have some rounding error that would loose CLOCKS. See what is not
    // being used. As mentioned above, this could result in putting back more than
    // is taken out
#ifdef _ARM_
    tmp = (adjusted * BOTTOM);
    do_div(tmp, TOP);
    adjusted = tmp;
#else
    adjusted = (adjusted * BOTTOM) / TOP;
#endif

    // If adjusted is not the same as timeDiff, then there is some rounding
    // error that needs to be pushed back into the previous sample.
    // NOTE: the following is so that the fact that everything is signed will not
    // matter.
    s_realTimePrevious = (clock_t)((int64_t)s_realTimePrevious - adjusted);
    s_realTimePrevious += timeDiff;

#ifdef  DEBUGGING_TIME
    // Put this in so that TPM time will pass much faster than real time when
    // doing debug.
    // A value of 1000 for DEBUG_TIME_MULTIPLER will make each ms into a second
    // A good value might be 100
    return (s_tpmTime * DEBUG_TIME_MULTIPLIER);
#endif
    return s_tpmTime;
#endif
}



//*** _plat__TimerWasReset()
// This function is used to interrogate the flag indicating if the tick timer has 
// been reset.
//
// If the resetFlag parameter is SET, then the flag will be CLEAR before the 
// function returns.
LIB_EXPORT BOOL
_plat__TimerWasReset(
   void          
    )
{
    BOOL         retVal = s_timerReset;
    s_timerReset = FALSE;
    return retVal;
}

//*** _plat__TimerWasStopped()
// This function is used to interrogate the flag indicating if the tick timer has 
// been stopped. If so, this is typically a reason to roll the nonce.
//
// This function will CLEAR the s_timerStopped flag before returning. This provides
// functionality that is similar to status register that is cleared when read. This
// is the model used here because it is the one that has the most impact on the TPM
// code as the flag can only be accessed by one entity in the TPM. Any other
// implementation of the hardware can be made to look like a read-once register.
LIB_EXPORT BOOL
_plat__TimerWasStopped(
    void
    )
{
    BOOL         retVal = s_timerStopped;
    s_timerStopped = FALSE;
    return retVal;
}

//***_plat__ClockAdjustRate()
// Adjust the clock rate
LIB_EXPORT void
_plat__ClockAdjustRate(
    int              adjust         // IN: the adjust number.  It could be positive
                                    //     or negative
    )
{
    // We expect the caller should only use a fixed set of constant values to
    // adjust the rate
    switch(adjust)
    {
        case CLOCK_ADJUST_COARSE:
            s_adjustRate += CLOCK_ADJUST_COARSE;
            break;
        case -CLOCK_ADJUST_COARSE:
            s_adjustRate -= CLOCK_ADJUST_COARSE;
            break;
        case CLOCK_ADJUST_MEDIUM:
            s_adjustRate += CLOCK_ADJUST_MEDIUM;
            break;
        case -CLOCK_ADJUST_MEDIUM:
            s_adjustRate -= CLOCK_ADJUST_MEDIUM;
            break;
        case CLOCK_ADJUST_FINE:
            s_adjustRate += CLOCK_ADJUST_FINE;
            break;
        case -CLOCK_ADJUST_FINE:
            s_adjustRate -= CLOCK_ADJUST_FINE;
            break;
        default:
            // ignore any other values;
            break;
    }

    if(s_adjustRate > (CLOCK_NOMINAL + CLOCK_ADJUST_LIMIT))
        s_adjustRate = CLOCK_NOMINAL + CLOCK_ADJUST_LIMIT;
    if(s_adjustRate < (CLOCK_NOMINAL - CLOCK_ADJUST_LIMIT))
        s_adjustRate = CLOCK_NOMINAL - CLOCK_ADJUST_LIMIT;

    return;
}

