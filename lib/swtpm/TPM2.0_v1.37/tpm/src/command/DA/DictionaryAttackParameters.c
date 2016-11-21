/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include "Tpm.h"
#include "DictionaryAttackParameters_fp.h"

#ifdef TPM_CC_DictionaryAttackParameters  // Conditional expansion of this file

/*(See part 3 specification)
// change the lockout parameters
*/
TPM_RC
TPM2_DictionaryAttackParameters(
    DictionaryAttackParameters_In   *in             // IN: input parameter list
    )
{
    // The command needs NV update.
    RETURN_IF_NV_IS_NOT_AVAILABLE;

// Internal Data Update

    // Set dictionary attack parameters
    gp.maxTries = in->newMaxTries;
    gp.recoveryTime = in->newRecoveryTime;
    gp.lockoutRecovery = in->lockoutRecovery;

    // Set failed tries to 0
    gp.failedTries = 0;

    // Record the changes to NV
    NV_SYNC_PERSISTENT(failedTries);
    NV_SYNC_PERSISTENT(maxTries);
    NV_SYNC_PERSISTENT(recoveryTime);
    NV_SYNC_PERSISTENT(lockoutRecovery);

    return TPM_RC_SUCCESS;
}

#endif // CC_DictionaryAttackParameters