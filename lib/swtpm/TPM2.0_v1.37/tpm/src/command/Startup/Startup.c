/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include "Tpm.h"
#include "Startup_fp.h"

#ifdef TPM_CC_Startup  // Conditional expansion of this file

/*(See part 3 specification)
// Initialize TPM because a system-wide reset
*/
// return type: TPM_RC
//      TPM_RC_LOCALITY             a Startup(STATE) does not have the same H-CRTM
//                                  state as the previous Startup() or the locality
//                                  of the startup is not 0 pr 3
//      TPM_RC_NV_UNINITIALIZED     the saved state cannot be recovered and a
//                                  Startup(CLEAR) is required.
//      TPM_RC_VALUE                start up type is not compatible with previous
//                                  shutdown sequence

TPM_RC
TPM2_Startup(
    Startup_In      *in             // IN: input parameter list
    )
{
    STARTUP_TYPE         startup;
    BYTE                 locality = _plat__LocalityGet();

    // The command needs NV update.
    RETURN_IF_NV_IS_NOT_AVAILABLE;

    // Get the flags for the current startup locality and the H-CRTM.
    // Rather than generalizing the locality setting, this code takes advantage
    // of the fact that the PC Client specification only allows Startup() 
    // from locality 0 and 3. To generalize this probably would require a 
    // redo of the NV space and since this is a feature that is hardly ever used
    // outside of the PC Client, this code just support the PC Client needs.

// Input Validation
    // Check that the locality is a supported value
    if(locality != 0 && locality != 3)
        return TPM_RC_LOCALITY;
    // If there was a H-CRTM, then treat the locality as being 3 
    // regardless of what the Startup() was. This is done to preserve the
    // H-CRTM PCR so that they don't get overwritten with the normal
    // PCR startup initialization. This basically means that g_StartupLocality3
    // and g_DrtmPreStartup can't both be SET at the same time.
    if(g_DrtmPreStartup)
        locality = 0;
    g_StartupLocality3 = (locality == 3);

#ifdef USE_DA_USED
    // If there was no orderly shutdown, then their might have been a write to
    // failedTries that didn't get recorded but only if g_daUsed was SET in the
    // shutdown state
    g_daUsed = (gp.orderlyState == SU_DA_USED_VALUE);
    if(g_daUsed)
        gp.orderlyState = SU_NONE_VALUE;
#endif
    g_prevOrderlyState = gp.orderlyState;

    // If this is a Resume, 
    if(in->startupType == TPM_SU_STATE)
    {
        // Turn of the startup modifiers in the recovered state. This will modify 
        // the SU_NONE_VALUE but not make it anything that would be recognized as 
        // a valid shutdown
        g_prevOrderlyState &= ~(PRE_STARTUP_FLAG | STARTUP_LOCALITY_3);

        // then there must have been a prior TPM2_ShutdownState(STATE) 
        if(g_prevOrderlyState != TPM_SU_STATE)
            return TPM_RCS_VALUE + RC_Startup_startupType;
        // and the part of NV used for state save must have been recovered 
        // correctly.
        // NOTE: if this fails, then the caller will need to do Startup(CLEAR). The
        // code for Startup(Clear) cannot fail if the NV can't be read correctly
        // because that would prevent the TPM from ever getting unstuck.
        if(g_nvOk == FALSE)
            return TPM_RC_NV_UNINITIALIZED;

        // For Resume, the H-CRTM has to be the same as the previous boot
        if(g_DrtmPreStartup != ((gp.orderlyState & PRE_STARTUP_FLAG) != 0))
            return TPM_RCS_VALUE + RC_Startup_startupType;
        if(g_StartupLocality3 != ((gp.orderlyState & STARTUP_LOCALITY_3) != 0))
            return TPM_RC_LOCALITY;
        gp.orderlyState = g_prevOrderlyState;
    }
    
// Internal Date Update
    if((gp.orderlyState == TPM_SU_STATE) && (g_nvOk == TRUE))
    {
        // Always read the data that is only cleared on a Reset because this is not
        // a reset
        NvRead(&gr, NV_STATE_RESET_DATA, sizeof(gr));
        if(in->startupType == TPM_SU_STATE)
        {
            // If this is a startup STATE (a Resume) need to read the data
            // that is cleared on a startup CLEAR because this is not a Reset
            // or Restart.
            NvRead(&gc, NV_STATE_CLEAR_DATA, sizeof(gc));
            startup = SU_RESUME;
        }
        else
            startup = SU_RESTART;
    }
    else
        // Will do a TPM reset if Shutdown(CLEAR) and Startup(CLEAR) or no shutdown
        // or there was a failure reading the NV data. 
        startup = SU_RESET;

    // Startup for cryptographic library. Don't do this until after the orderly
    // state has been read in from NV.
    CryptStartup(startup);

    // Read the platform unique value that is used as VENDOR_PERMANENT
    // authorization value
    g_platformUniqueDetails.t.size 
        = (UINT16)_plat__GetUnique(1, sizeof(g_platformUniqueDetails.t.buffer),
                                   g_platformUniqueDetails.t.buffer);

// Start up subsystems
    // Start set the safe flag
    TimeStartup(startup);

    // Start dictionary attack subsystem
    DAStartup(startup);

    // Enable hierarchies
    HierarchyStartup(startup);

    // Restore/Initialize PCR
    PCRStartup(startup, locality);

    // Restore/Initialize command audit information
    CommandAuditStartup(startup);

//// The following code was moved from Time.c where it made no sense
    switch (startup)
    {
        case SU_RESUME:
            // Resume sequence
            gr.restartCount++;
            break;
        case SU_RESTART:
            // Hibernate sequence
            gr.clearCount++;
            gr.restartCount++;
            break;
        default:
            // Reset object context ID to 0
            gr.objectContextID = 0;
            // Reset clearCount to 0
            gr.clearCount = 0;

            // Reset sequence
            // Increase resetCount
            gp.resetCount++;

            // Write resetCount to NV
            NV_SYNC_PERSISTENT(resetCount);

            gp.totalResetCount++;
            // We do not expect the total reset counter overflow during the life
            // time of TPM.  if it ever happens, TPM will be put to failure mode
            // and there is no way to recover it.
            // The reason that there is no recovery is that we don't increment
            // the NV totalResetCount when incrementing would make it 0. When the
            // TPM starts up again, the old value of totalResetCount will be read
            // and we will get right back to here with the increment failing.
            if(gp.totalResetCount == 0)
                FAIL(FATAL_ERROR_INTERNAL);

            // Write total reset counter to NV
            NV_SYNC_PERSISTENT(totalResetCount);

            // Reset restartCount
            gr.restartCount = 0;
            
            break;
    }
////

    // Initialize session table
    SessionStartup(startup);

    // Initialize object table
    ObjectStartup();

    // Initialize index/evict data.  This function clears read/write locks
    // in NV index
    NvEntityStartup(startup);

    // Initialize the orderly shut down flag for this cycle to SU_NONE_VALUE.
    gp.orderlyState = SU_NONE_VALUE;

    NV_SYNC_PERSISTENT(orderlyState);

    // Update TPM internal states if command succeeded.
    // Record a TPM2_Startup command has been received.
    TPMRegisterStartup();
   
    // This can be reset after the first completion of a TPM2_Startup() after
    // a power loss. It can probably be reset earlier but this is an OK place.
    g_powerWasLost = FALSE;

    return TPM_RC_SUCCESS;
}

#endif // CC_Startup