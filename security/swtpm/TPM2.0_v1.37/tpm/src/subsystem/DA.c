/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
// This file contains the functions and data definitions relating to the
// dictionary attack logic.

//** Includes and Data Definitions
#define DA_C
#include "Tpm.h"

//** Functions

//*** DAPreInstall_Init()
// This function initializes the DA parameters to their manufacturer-default
// values. The default values are determined by a platform-specific specification.
//
// This function should not be called outside of a manufacturing or simulation
// environment.
//
// The DA parameters will be restored to these initial values by TPM2_Clear().
void
DAPreInstall_Init(
    void
    )
{
    gp.failedTries = 0;
    gp.maxTries = 3;
    gp.recoveryTime = 1000;         // in seconds (~16.67 minutes)
    gp.lockoutRecovery = 1000;      // in seconds
    gp.lockOutAuthEnabled = TRUE;   // Use of lockoutAuth is enabled

    // Record persistent DA parameter changes to NV
    NV_SYNC_PERSISTENT(failedTries);
    NV_SYNC_PERSISTENT(maxTries);
    NV_SYNC_PERSISTENT(recoveryTime);
    NV_SYNC_PERSISTENT(lockoutRecovery);
    NV_SYNC_PERSISTENT(lockOutAuthEnabled);

    return;
}

//*** DAInit()
// This function is called during _TPM_INIT in order to make sure that
// the DA timers are reset when g_time is reset.
void
DAInit(
    void
    )
{
    s_selfHealTimer = g_time;
    s_lockoutTimer = g_time;
}

//*** DAStartup()
// This function is called  by TPM2_Startup() to initialize the DA parameters.
// In the case of Startup(CLEAR), use of lockoutAuth will be enabled if the
// lockout recovery time is 0. Otherwise, lockoutAuth will not be enabled until
// the TPM has been continuously powered for the lockoutRecovery time.
//
// This function requires that NV be available and not rate limiting.
void
DAStartup(
    STARTUP_TYPE     type           // IN: startup type
    )
{
    NOT_REFERENCED(type);
    // For any Startup(), if lockoutRecovery is 0, enable use of lockoutAuth.
    if(gp.lockoutRecovery == 0)
    {
        gp.lockOutAuthEnabled = TRUE;
        // Record the changes to NV
        NV_SYNC_PERSISTENT(lockOutAuthEnabled);
    }

    // If DA has not been disabled and the previous shutdown is not orderly
    // failedTries is not already at its maximum then increment 'failedTries'
    if(gp.recoveryTime != 0
       && gp.failedTries < gp.maxTries
       && !IS_ORDERLY(g_prevOrderlyState))
    {
#ifdef USE_DA_USED
        gp.failedTries += g_daUsed;
        g_daUsed = FALSE;
#else
        gp.failedTries++;
#endif
        // Record the change to NV
        NV_SYNC_PERSISTENT(failedTries);
    }

    return;
}

//*** DARegisterFailure()
// This function is called when a authorization failure occurs on an entity
// that is subject to dictionary-attack protection. When a DA failure is
// triggered, register the failure by resetting the relevant self-healing
// timer to the current time.
void
DARegisterFailure(
    TPM_HANDLE       handle         // IN: handle for failure
    )
{
    // Reset the timer associated with lockout if the handle is the lockoutAuth.
    if(handle == TPM_RH_LOCKOUT)
        s_lockoutTimer = g_time;
    else
        s_selfHealTimer = g_time;
    return;
}

//*** DASelfHeal()
// This function is called to check if sufficient time has passed to allow
// decrement of failedTries or to re-enable use of lockoutAuth.
//
// This function should be called when the time interval is updated.
void
DASelfHeal(
    void
    )
{
    // Regular authorization self healing logic
    // If no failed authorization tries, do nothing.  Otherwise, try to
    // decrease failedTries
    if(gp.failedTries != 0)
    {
        // if recovery time is 0, DA logic has been disabled.  Clear failed tries
        // immediately
        if(gp.recoveryTime == 0)
        {
            gp.failedTries = 0;
            // Update NV record
            NV_SYNC_PERSISTENT(failedTries);
        }
        else
        {
            UINT64          decreaseCount;
#ifdef _ARM_
            UINT64          tmp;
#endif

            // In the unlikely event that failedTries should become larger than
            // maxTries
            if(gp.failedTries > gp.maxTries)
                gp.failedTries = gp.maxTries;

            // How much can failedTries be decreased
#ifdef _ARM_
            tmp = (g_time - s_selfHealTimer);
            do_div(tmp, 1000);
            do_div(tmp, gp.recoveryTime);
#else
            decreaseCount = ((g_time - s_selfHealTimer) / 1000) / gp.recoveryTime;
#endif

            if(gp.failedTries <= (UINT32)decreaseCount)
                // should not set failedTries below zero
                gp.failedTries = 0;
            else
                gp.failedTries -= (UINT32)decreaseCount;

            // the cast prevents overflow of the product
            s_selfHealTimer += (decreaseCount * (UINT64)gp.recoveryTime) * 1000;
            if(decreaseCount != 0)
                // If there was a change to the failedTries, record the changes
                // to NV
                NV_SYNC_PERSISTENT(failedTries);
        }
    }

    // LockoutAuth self healing logic
    // If lockoutAuth is enabled, do nothing.  Otherwise, try to see if we
    // may enable it
    if(!gp.lockOutAuthEnabled)
    {
        // if lockout authorization recovery time is 0, a reboot is required to
        // re-enable use of lockout authorization.  Self-healing would not
        // apply in this case.
        if(gp.lockoutRecovery != 0)
        {
#ifdef _ARM_
            UINT64 tmp;
            tmp = g_time - s_lockoutTimer;
            do_div(tmp, 1000);
            if (tmp >= gp.lockoutRecovery)
#else
            if(((g_time - s_lockoutTimer) / 1000) >= gp.lockoutRecovery)
#endif
            {
                gp.lockOutAuthEnabled = TRUE;
                // Record the changes to NV
                NV_SYNC_PERSISTENT(lockOutAuthEnabled);
            }
        }
    }

    return;
}
