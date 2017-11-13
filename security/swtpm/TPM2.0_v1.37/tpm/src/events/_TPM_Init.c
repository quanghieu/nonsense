/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include "Tpm.h"
#include "_TPM_Init_fp.h"

 // This function is used to process a _TPM_Init indication.
LIB_EXPORT void
_TPM_Init(
    void
    )
{
    g_powerWasLost = g_powerWasLost | _plat__WasPowerLost();

#if defined SIMULATION && !defined NDEBUG
    // If power was lost and this was a simulation, put canary in RAM used by NV
    // so that uninitialized memory can be detected more easily
    if(g_powerWasLost)
    {
        memset(&gc, 0xbb, sizeof(gc));
        memset(&gr, 0xbb, sizeof(gr));
        memset(&gp, 0xbb, sizeof(gp));
        memset(&go, 0xbb, sizeof(go));
    }
#endif   

#ifdef SIMULATION
    // Clear the flag that forces failure on self-test
    g_forceFailureMode = FALSE;
#endif

    // Set initialization state
    TPMInit();

    // Set g_DRTMHandle as unassigned
    g_DRTMHandle = TPM_RH_UNASSIGNED;

    // No H-CRTM, yet.
    g_DrtmPreStartup = FALSE;

    // Initialize the NvEnvironment.
    g_nvOk = NvPowerOn();
    
    // Initialize cryptographic functions
    g_inFailureMode = (CryptInit() == FALSE);
    if(!g_inFailureMode)
    {
        // Load the persistent data
        NvReadPersistent();

        // Load the orderly data (clock and DRBG state).
        // If this is not done here, things break
        NvRead(&go, NV_ORDERLY_DATA, sizeof(go));

        // Start clock. Need to do this after NV has been restored.
//        TimePowerOn();
    }
    return;
}
