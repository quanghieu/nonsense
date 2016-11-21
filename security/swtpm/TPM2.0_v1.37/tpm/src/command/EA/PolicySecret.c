/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include "Tpm.h"
#include "PolicySecret_fp.h"

#ifdef TPM_CC_PolicySecret  // Conditional expansion of this file

#include "Policy_spt_fp.h"
#include "NV_spt_fp.h"

/*(See part 3 specification)
// Add a secret-based authorization to the policy evaluation
*/
// return type: TPM_RC
//      TPM_RC_CPHASH           cpHash for policy was previously set to a
//                              value that is not the same as 'cpHashA'
//      TPM_RC_EXPIRED          'expiration' indicates a time in the past
//      TPM_RC_NONCE            'nonceTPM' does not match the nonce associated
//                              with 'policySession'
//      TPM_RC_SIZE             'cpHashA' is not the size of a digest for the
//                              hash associated with 'policySession'
TPM_RC
TPM2_PolicySecret(
    PolicySecret_In     *in,            // IN: input parameter list
    PolicySecret_Out    *out            // OUT: output parameter list
    )
{
    TPM_RC                   result;
    SESSION                 *session;
    TPM2B_NAME               entityName;
    UINT64                   authTimeout = 0;
// Input Validation
    // Get pointer to the session structure
    session = SessionGet(in->policySession);

    //Only do input validation if this is not a trial policy session
    if(session->attributes.isTrialPolicy == CLEAR)
    {
        authTimeout = ComputeAuthTimeout(session, in->expiration, &in->nonceTPM);

        result = PolicyParameterChecks(session, authTimeout,
                                       &in->cpHashA, &in->nonceTPM,
                                       RC_PolicySecret_nonceTPM,
                                       RC_PolicySecret_cpHashA,
                                       RC_PolicySecret_expiration);
        if(result != TPM_RC_SUCCESS)
            return result;
    }
// Internal Data Update
    // Update policy context with input policyRef and name of authorizing key
    // This value is computed even for trial sessions. Possibly update the cpHash
    PolicyContextUpdate(TPM_CC_PolicySecret,
                        EntityGetName(in->authHandle, &entityName), &in->policyRef,
                        &in->cpHashA, authTimeout, session);
// Command Output
    // Create ticket and timeout buffer if in->expiration < 0 and this is not
    // a trial session.
    // NOTE: PolicyParameterChecks() makes sure that nonceTPM is present
    // when expiration is non-zero.
    if(in->expiration < 0
       && session->attributes.isTrialPolicy == CLEAR
       && !NvIsPinPassIndex(in->authHandle))
    {
        BOOL        expiresOnReset = (in->nonceTPM.t.size == 0);
        // Generate timeout buffer.  The format of output timeout buffer is
        // TPM-specific.
        // In this implementation, the timeout parameter is the timeout relative
        // to g_time with a one byte flag to indicate if the ticket will expire on 
        // TPM Reset
        out->timeout.t.size = sizeof(authTimeout) + 1;
        UINT64_TO_BYTE_ARRAY(authTimeout, out->timeout.t.buffer);
        out->timeout.t.buffer[sizeof(authTimeout)] = (BYTE)expiresOnReset;
        
        // Compute policy ticket
        TicketComputeAuth(TPM_ST_AUTH_SECRET, EntityGetHierarchy(in->authHandle),
                          authTimeout, expiresOnReset, &in->cpHashA, &in->policyRef,
                          &entityName, &out->policyTicket);
    }
    else
    {
        // timeout buffer is null
        out->timeout.t.size = 0;

        // authorization ticket is null
        out->policyTicket.tag = TPM_ST_AUTH_SECRET;
        out->policyTicket.hierarchy = TPM_RH_NULL;
        out->policyTicket.digest.t.size = 0;
    }
    return TPM_RC_SUCCESS;
}

#endif // CC_PolicySecret