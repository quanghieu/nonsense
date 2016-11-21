/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include "Tpm.h"
#include "Unseal_fp.h"

#ifdef TPM_CC_Unseal  // Conditional expansion of this file

/*(See part 3 specification)
// return data in a sealed data blob
*/
// return type: TPM_RC
//    TPM_RC_ATTRIBUTES           'itemHandle' has wrong attributes
//    TPM_RC_TYPE                 'itemHandle' is not a KEYEDHASH data object
TPM_RC
TPM2_Unseal(
    Unseal_In           *in,
    Unseal_Out          *out
    )
{
    OBJECT                  *object;

// Input Validation

    // Get pointer to loaded object
    object = HandleToObject(in->itemHandle);

    // Input handle must be a data object
    if(object->publicArea.type != TPM_ALG_KEYEDHASH)
        return TPM_RCS_TYPE + RC_Unseal_itemHandle;
    if(object->publicArea.objectAttributes.decrypt == SET
       || object->publicArea.objectAttributes.sign == SET
       || object->publicArea.objectAttributes.restricted == SET)
        return TPM_RCS_ATTRIBUTES + RC_Unseal_itemHandle;

// Command Output

    // Copy data
    out->outData = object->sensitive.sensitive.bits;
    return TPM_RC_SUCCESS;
}

#endif // CC_Unseal