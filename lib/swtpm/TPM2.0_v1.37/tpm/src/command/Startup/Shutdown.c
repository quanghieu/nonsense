/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include "Tpm.h"
#include "Shutdown_fp.h"

#ifdef TPM_CC_Shutdown  // Conditional expansion of this file

/*(See part 3 specification)
// Shut down TPM for power off
*/
// return type: TPM_RC
//      TPM_RC_TYPE             if PCR bank has been re-configured, a
//                              CLEAR StateSave is required
TPM_RC
TPM2_Shutdown(
    Shutdown_In     *in             // IN: input parameter list
    )
{
    // The command needs NV update.  Check if NV is available.
    // A TPM_RC_NV_UNAVAILABLE or TPM_RC_NV_RATE error may be returned at
    // this point
    RETURN_IF_NV_IS_NOT_AVAILABLE;

// Input Validation

    // If PCR bank has been reconfigured, a CLEAR state save is required
    if(g_pcrReConfig && in->shutdownType == TPM_SU_STATE)
        return TPM_RCS_TYPE + RC_Shutdown_shutdownType;

// Internal Data Update

    gp.orderlyState = in->shutdownType;

    // PCR private date state save
    PCRStateSave(in->shutdownType);

    // Save RAM backed NV index data
    NvUpdateIndexOrderlyData();

    // Save all orderly data
    NvWrite(NV_ORDERLY_DATA, sizeof(ORDERLY_DATA), &go);

    if(in->shutdownType == TPM_SU_STATE)
    {
        // Save STATE_RESET and STATE_CLEAR data
        NvWrite(NV_STATE_CLEAR_DATA, sizeof(STATE_CLEAR_DATA), &gc);
        NvWrite(NV_STATE_RESET_DATA, sizeof(STATE_RESET_DATA), &gr);

        // Save the startup flags for resume
        if(g_DrtmPreStartup)
            gp.orderlyState = TPM_SU_STATE | PRE_STARTUP_FLAG;
        else if(g_StartupLocality3)
            gp.orderlyState = TPM_SU_STATE | STARTUP_LOCALITY_3;
    }
    else if(in->shutdownType == TPM_SU_CLEAR)
    {
        // Save STATE_RESET data
        NvWrite(NV_STATE_RESET_DATA, sizeof(STATE_RESET_DATA), &gr);
    }
    else
        FAIL(FATAL_ERROR_INTERNAL);

    NV_SYNC_PERSISTENT(orderlyState);

    return TPM_RC_SUCCESS;
}

#endif // CC_Shutdown