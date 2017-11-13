/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
// This file contains the functions relating to the TPM's time functions including
// the interface to the implementation-specific time functions.
//
//** Includes
#include "Tpm.h"
#include "PlatformData.h"

//** Functions

//*** TimePowerOn()
// This function initialize time info at _TPM_Init().
//
// This function is called at _TPM_Init() so that the TPM time can start counting
// as soon as the TPM comes out of reset and doesn't have to wait until
// TPM2_Startup() in order to begin the new time epoch. This could be significant
// for systems that could get powered up but not run any TPM commands for some
// period of time. 
//
void
TimePowerOn(
    void
    )
{
    // If the timer was reset or stopped, we need a new epoch 
    if(_plat__TimerWasReset())
    {
        g_timeNewEpochNeeded = TRUE;
        // If the timer was reset, need to reset the base time of the TPM. By
        // resetting to zero here, the TPM can capture the time that passed between
        // when the system timer was reset and when the first call is made to 
        // _plat__TimeRead().
        g_time = 0;
        // And reset the DA timers
        DAInit();
    }
}

//*** TimeNewEpoch()
// This function does the processing to generate a new time epoch nonce and
// set NV for update. This function is only called when NV is known to be available
// and the clock is running. The epoch is updated to persistent data.
static void
TimeNewEpoch(
    void
    )
{
#ifdef CLOCK_STOPS
    CryptRandomGenerate(sizeof(CLOCK_NONCE), (BYTE *)&g_timeEpoch);
#else
    // if the epoch is kept in NV, update it.
    gp.timeEpoch++;
    NV_SYNC_PERSISTENT(timeEpoch);
#endif
    g_timeNewEpochNeeded = FALSE;
    // Clean out any lingering state
    _plat__TimerWasStopped();
}

//*** TimeStartup()
// This function updates the resetCount and restartCount components of
// TPMS_CLOCK_INFO structure at TPM2_Startup().
//
// This function will deal with the deferred creation of a new epoch. 
// TimeUpdateToCurrent() will not start a new epoch even if one is due when
// TPM_Startup() has not been run. This is because the state of NV is not known
// until startup completes. When Startup is done, then it will create the epoch
// nonce to complete the initializations by calling this function.
void
TimeStartup(
    STARTUP_TYPE     type           // IN: start up type
    )
{
    NOT_REFERENCED(type);
    // If the previous cycle is orderly shut down, the value of the safe bit
    // the same as previously saved.  Otherwise, it is not safe.
    if(!NV_IS_ORDERLY)
        go.clockSafe = NO;

    // Before Startup, the TPM will not do clock updates. At startup, need to
    // do a time update.
//    TimeUpdate();
    return;
}

//***TimeClockUpdate()
// This function updates go.clock. If 'newTime' requires an update of NV, then
// NV is checked for availability. If it is not available or is rate limiting, then
// go.clock is not updated and the function returns an error. If 'newTime' would
// not cause an NV write, then go.clock is updated. If an NV write occurs, then
// go.safe is SET.
// return type: TPM_RC
//  TPM_RC_NV_RATE          NV cannot be written because it is rate limiting
//  TPM_RC_NV_UNAVAILABLE   NV cannot be accessed
TPM_RC
TimeClockUpdate(
    UINT64           newTime
    )
{
#define CLOCK_UPDATE_MASK  ((1ULL << NV_CLOCK_UPDATE_INTERVAL)- 1)

    // Check to see if the update will cause a need for an nvClock update
    if((newTime | CLOCK_UPDATE_MASK) > (go.clock | CLOCK_UPDATE_MASK))
    {
        RETURN_IF_NV_IS_NOT_AVAILABLE;

        // Going to update the NV time state so SET the safe flag
        go.clockSafe = YES;

        // update the time
        go.clock = newTime;

        NvWrite(NV_ORDERLY_DATA, sizeof(go), &go);
    }
    else
        // No NV update needed so just update
        go.clock = newTime;

    return TPM_RC_SUCCESS;
}

//*** TimeUpdate()
// This function is used to update the time and clock values. If the TPM
// has run TPM2_Startup(), this function is called at the start of each command.
// If the TPM has not run TPM2_Startup(), this is called from TPM2_Startup() to
// get the clock values initialized. It is not called on command entry because, in
// this implementation, the go structure is not read from NV until TPM2_Startup().
// The reason for this is that the initialization code (_TPM_Init()) may run before
// NV is accessible.
void
TimeUpdate(
    void
    )
{
        UINT64          elapsed;
//
    if(g_timeNewEpochNeeded)
        TimeNewEpoch();

    // Get the difference between this call and the last time we updated the tick
    // timer.
    elapsed = _plat__TimerRead() - g_time;
    g_time += elapsed;

    // Don't need to check the result because it has to be success because have
    // already checked that NV is available.
    TimeClockUpdate(go.clock + elapsed);

    // Call self healing logic for dictionary attack parameters
    DASelfHeal();


}

//*** TimeUpdateToCurrent()
// This function updates the 'Time' and 'Clock' in the global
// TPMS_TIME_INFO structure.
//
// In this implementation, 'Time' and 'Clock' are updated at the beginning
// of each command and the values are unchanged for the duration of the
// command.
//
// Because 'Clock' updates may require a write to NV memory, 'Time' and 'Clock'
// are not allowed to advance if NV is not available. When clock is not advancing,
// any function that uses 'Clock' will fail and return TPM_RC_NV_UNAVAILABLE or
// TPM_RC_NV_RATE.
//
// This implementation does not do rate limiting. If the implementation does do
// rate limiting, then the 'Clock' update should not be inhibited even when doing
// rate limiting.
void
TimeUpdateToCurrent(
    void
    )
{
    UINT64          elapsed;
//
    // Can't update time during the dark interval or when rate limiting so don't
    // make any modifications to the internal clock value
    if(!NV_IS_AVAILABLE)
        return;
    // Make sure that we consume the current _plat__TimerWasStopped() state.
    g_timeNewEpochNeeded |= _plat__TimerWasStopped();

    // If we need a new epoch but the TPM has not started, don't generate the new
    // epoch here because the crypto has not been initialized by TPM2_Startup().
    // Instead, just continue and let TPM2_Startup() processing create the
    // new epoch if needed.
    if(g_timeNewEpochNeeded && TPMIsStarted())
    {
        TimeNewEpoch();
    }
    // Get the difference between this call and the last time we updated the tick
    // timer.
    elapsed = _plat__TimerRead() - g_time;
    g_time += elapsed;

    // Don't need to check the result because it has to be success because have
    // already checked that NV is available.
    TimeClockUpdate(go.clock + elapsed);

    // Call self healing logic for dictionary attack parameters
    DASelfHeal();

    return;
}

//*** TimeSetAdjustRate()
// This function is used to perform rate adjustment on 'Time' and 'Clock'.
void
TimeSetAdjustRate(
    TPM_CLOCK_ADJUST     adjust         // IN: adjust constant
    )
{
    switch(adjust)
    {
        case TPM_CLOCK_COARSE_SLOWER:
            _plat__ClockAdjustRate(CLOCK_ADJUST_COARSE);
            break;
        case TPM_CLOCK_COARSE_FASTER:
            _plat__ClockAdjustRate(-CLOCK_ADJUST_COARSE);
            break;
        case TPM_CLOCK_MEDIUM_SLOWER:
            _plat__ClockAdjustRate(CLOCK_ADJUST_MEDIUM);
            break;
        case TPM_CLOCK_MEDIUM_FASTER:
            _plat__ClockAdjustRate(-CLOCK_ADJUST_MEDIUM);
            break;
        case TPM_CLOCK_FINE_SLOWER:
            _plat__ClockAdjustRate(CLOCK_ADJUST_FINE);
            break;
        case TPM_CLOCK_FINE_FASTER:
            _plat__ClockAdjustRate(-CLOCK_ADJUST_FINE);
            break;
        case TPM_CLOCK_NO_CHANGE:
            break;
        default:
            FAIL(FATAL_ERROR_INTERNAL);
            break;
    }

    return;
}

//*** TimeGetMarshaled()
// This function is used to access TPMS_TIME_INFO in canonical form.
// The function collects the time information and marshals it into 'dataBuffer'
// and returns the marshaled size
// return type: UINT16
UINT16
TimeGetMarshaled(
    TIME_INFO       *dataBuffer     // OUT: result buffer
    )
{
    TPMS_TIME_INFO      timeInfo;

    // Fill TPMS_TIME_INFO structure
    timeInfo.time = g_time;
    TimeFillInfo(&timeInfo.clockInfo);

    // Marshal TPMS_TIME_INFO to canonical form
    return TPMS_TIME_INFO_Marshal(&timeInfo, (BYTE **)&dataBuffer, NULL);
}

//*** TimeFillInfo
// This function gathers information to fill in a TPMS_CLOCK_INFO structure.
void
TimeFillInfo(
    TPMS_CLOCK_INFO     *clockInfo
    )
{
    clockInfo->clock = go.clock;
    clockInfo->resetCount = gp.resetCount;
    clockInfo->restartCount = gr.restartCount;

    // If NV is not available, clock stopped advancing and the value reported is
    // not "safe".
    if(NV_IS_AVAILABLE)
        clockInfo->safe = go.clockSafe;
    else
        clockInfo->safe = NO;

    return;
}
