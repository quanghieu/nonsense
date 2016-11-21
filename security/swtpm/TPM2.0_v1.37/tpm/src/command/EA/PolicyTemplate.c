/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include "Tpm.h"
#include "PolicyTemplate_fp.h"

#ifdef TPM_CC_PolicyTemplate  // Conditional expansion of this file

/*(See part 3 specification)
// Add a cpHash restriction to the policyDigest
*/
// return type: TPM_RC
//      TPM_RC_CPHASH           cpHash of 'policySession' has previously been set
//                              to a different value
//      TPM_RC_SIZE             'templateHash' is not the size of a digest produced
//                              by the hash algorithm associated with
//                              'policySession'
TPM_RC
TPM2_PolicyTemplate(
    PolicyTemplate_In     *in             // IN: input parameter list
    )
{
    SESSION     *session;
    TPM_CC      commandCode = TPM_CC_PolicyTemplate;
    HASH_STATE  hashState;

// Input Validation

    // Get pointer to the session structure
    session = SessionGet(in->policySession);

    // If the template is set, make sure that it is the same as the input value
    if(session->attributes.isTemplateSet)
    {
        if(!MemoryEqual2B(&in->templateHash.b, &session->u1.cpHash.b))
            return TPM_RCS_VALUE + RC_PolicyTemplate_templateHash;
    }
    // error if cpHash contains something that is not a template
    else if(session->u1.templateHash.t.size != 0)
        return TPM_RCS_VALUE + RC_PolicyTemplate_templateHash;

    // A valid templateHash must have the same size as session hash digest
    if(in->templateHash.t.size != CryptHashGetDigestSize(session->authHashAlg))
        return TPM_RCS_SIZE + RC_PolicyTemplate_templateHash;

// Internal Data Update
    // Update policy hash
    // policyDigestnew = hash(policyDigestold || TPM_CC_PolicyCpHash
    //  || cpHashA.buffer)
    //  Start hash
    CryptHashStart(&hashState, session->authHashAlg);

    //  add old digest
    CryptDigestUpdate2B(&hashState, &session->u2.policyDigest.b);

    //  add commandCode
    CryptDigestUpdateInt(&hashState, sizeof(TPM_CC), commandCode);

    //  add cpHashA
    CryptDigestUpdate2B(&hashState, &in->templateHash.b);

    //  complete the digest and get the results
    CryptHashEnd2B(&hashState, &session->u2.policyDigest.b);

    // update cpHash in session context
    session->u1.templateHash = in->templateHash;
    session->attributes.isTemplateSet = SET;

    return TPM_RC_SUCCESS;
}

#endif // CC_PolicyTemplateHash