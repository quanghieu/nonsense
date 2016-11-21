/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include "Tpm.h"
#include "ChangePPS_fp.h"

#ifdef TPM_CC_ChangePPS  // Conditional expansion of this file

/*(See part 3 specification)
// Reset current PPS value
*/
TPM_RC
TPM2_ChangePPS(
    ChangePPS_In    *in             // IN: input parameter list
    )
{
    UINT32          i;

    // Check if NV is available.  A TPM_RC_NV_UNAVAILABLE or TPM_RC_NV_RATE
    // error may be returned at this point
    RETURN_IF_NV_IS_NOT_AVAILABLE;

    // Input parameter is not reference in command action
    in = NULL;

// Internal Data Update

    // Reset platform hierarchy seed from RNG
    CryptRandomGenerate(PRIMARY_SEED_SIZE, gp.PPSeed.t.buffer);

    // Create a new phProof value from RNG to prevent the saved platform
    // hierarchy contexts being loaded
    CryptRandomGenerate(PROOF_SIZE, gp.phProof.t.buffer);

    // Set platform authPolicy to null
    gc.platformAlg = TPM_ALG_NULL;
    gc.platformPolicy.t.size = 0;

    // Flush loaded object in platform hierarchy
    ObjectFlushHierarchy(TPM_RH_PLATFORM);

    // Flush platform evict object and index in NV
    NvFlushHierarchy(TPM_RH_PLATFORM);

    // Save hierarchy changes to NV
    NV_SYNC_PERSISTENT(PPSeed);
    NV_SYNC_PERSISTENT(phProof);

    // Re-initialize PCR policies
#if defined NUM_POLICY_PCR_GROUP && NUM_POLICY_PCR_GROUP > 0
    for(i = 0; i < NUM_POLICY_PCR_GROUP; i++)
    {
        gp.pcrPolicies.hashAlg[i] = TPM_ALG_NULL;
        gp.pcrPolicies.policy[i].t.size = 0;
    }
    NV_SYNC_PERSISTENT(pcrPolicies);
#endif

    // orderly state should be cleared because of the update to state clear data
    g_clearOrderly = TRUE;

    return TPM_RC_SUCCESS;
}

#endif // CC_ChangePPS