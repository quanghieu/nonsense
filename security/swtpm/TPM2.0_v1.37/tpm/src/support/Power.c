/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Description

// This file contains functions that receive the simulated power state
// transitions of the TPM.

//** Includes and Data Definitions
#define POWER_C
#include "Tpm.h"

//** Functions

//*** TPMInit()
// This function is used to process a power on event.
#ifndef INLINE_FUNCTIONS
void
TPMInit(
    void
    )
{
    // Set state as not initialized. This means that Startup is required
    g_initialized = FALSE;
    return;
}
#endif  // INLINE_FUNCTIONS

//*** TPMRegisterStartup()
// This function registers the fact that the TPM has been initialized
// (a TPM2_Startup() has completed successfully).
#ifndef INLINE_FUNCTIONS
void
TPMRegisterStartup(
    void
    )
{
    g_initialized = TRUE;
    return;
}
#endif  // INLINE_FUNCTIONS

//*** TPMIsStarted()
// Indicates if the TPM has been initialized (a TPM2_Startup() has completed
// successfully after a _TPM_Init).
// return type: BOOL
//      TRUE        TPM has been initialized
//      FALSE       TPM has not been initialized
#ifndef INLINE_FUNCTIONS
BOOL
TPMIsStarted(
    void
    )
{
    return g_initialized;
}
#endif // INLINE_FUNCTIONS