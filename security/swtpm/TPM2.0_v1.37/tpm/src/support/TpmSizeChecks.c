/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Includes, Defines, and Types
#include    "Tpm.h"
#include    <assert.h>

#ifdef RUNTIME_SIZE_CHECKS

// ** TpmSizeChecks()
// This function is used during the development process to make sure that the
// vendor-specific values result in a consistent implementation. When possible,
// the code contains #if to do compile-time checks. However, in some cases, the 
// values require the use of "sizeof()" and that can't be used in an #if.
void
TpmSizeChecks(
    void
    )
{
    // Make sure that the size of the context blob is large enough for the largest
    // context
// TBD

    // Make sure that the size of the Capability buffer can hold the largest
    // TPML_PCR_SELECTION. The list length is nominally set by the number of hash
    // algorithms implemented on the TPM. A requirement of this implementation is 
    // that a list of all allowed TPMS_PCR_SELECTIONS fits in MAX_CAP_DATA.
// TBD
}

#endif // RUNTIME_SIZE_CHECKS