/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include "Tpm.h"
#include "NV_DefineSpace_fp.h"

#ifdef TPM_CC_NV_DefineSpace  // Conditional expansion of this file

/*(See part 3 specification)
// Define a NV index space
*/
// return type: TPM_RC
//      TPM_RC_HIERARCHY            for authorizations using TPM_RH_PLATFORM
//                                  phEnable_NV is clear preventing access to NV
//                                  data in the platform hierarchy.
//      TPM_RC_ATTRIBUTES           attributes of the index are not consistent
//      TPM_RC_NV_DEFINED           index already exists
//      TPM_RC_NV_SPACE             insufficient space for the index
//      TPM_RC_SIZE                 'auth->size' or 'publicInfo->authPolicy.size' is
//                                  larger than the digest size of
//                                  'publicInfo->nameAlg'; or 'publicInfo->dataSize'
//                                  is not consistent with 'publicInfo->attributes'
//                                  (this includes the case when the index is
//                                   larger than a MAX_NV_BUFFER_SIZE but the
//                                   TPMA_NV_WRITEALL attribute is SET)
TPM_RC
TPM2_NV_DefineSpace(
    NV_DefineSpace_In   *in             // IN: input parameter list
    )
{
    TPMA_NV         attributes = in->publicInfo.nvPublic.attributes;
    UINT16          nameSize;

    nameSize = CryptHashGetDigestSize(in->publicInfo.nvPublic.nameAlg);

// Input Validation

    // Checks not specific to type

    // If the UndefineSpaceSpecial command is not implemented, then can't have
    // an index that can only be deleted with policy
#if CC_NV_UndefineSpaceSpecial == NO
    if(IsNv_TPMA_NV_POLICY_DELETE(attributes))
        return TPM_RCS_ATTRIBUTES + RC_NV_DefineSpace_publicInfo;
#endif

    // check that the authPolicy consistent with hash algorithm

    if(in->publicInfo.nvPublic.authPolicy.t.size != 0
       && in->publicInfo.nvPublic.authPolicy.t.size != nameSize)
        return TPM_RCS_SIZE + RC_NV_DefineSpace_publicInfo;

   // make sure that the authValue is not too large
    if(MemoryRemoveTrailingZeros(&in->auth)
       > CryptHashGetDigestSize(in->publicInfo.nvPublic.nameAlg))
        return TPM_RCS_SIZE + RC_NV_DefineSpace_auth;

    // If an index is being created by the owner and shEnable is
    // clear, then we would not reach this point because ownerAuth
    // can't be given when shEnable is CLEAR. However, if phEnable
    // is SET but phEnableNV is CLEAR, we have to check here
    if(in->authHandle == TPM_RH_PLATFORM && gc.phEnableNV == CLEAR)
        return TPM_RCS_HIERARCHY + RC_NV_DefineSpace_authHandle;

    // Attribute checks
    // Eliminate the unsupported types
    switch(NV_ATTRIBUTES_TO_TYPE(attributes))
    {
#if CC_NV_Increment == YES
        case TPM_NT_COUNTER:
#endif
#if CC_NV_SetBits == YES
        case TPM_NT_BITS:
#endif
#if CC_NV_Extend == YES
        case TPM_NT_EXTEND:
#endif
#if CC_PolicySigned == YES && defined TPM_NT_PIN_PASS
        case TPM_NT_PIN_PASS:
        case TPM_NT_PIN_FAIL:
#endif
        case TPM_NT_ORDINARY:
            break;
        default:
            return TPM_RCS_ATTRIBUTES + RC_NV_DefineSpace_publicInfo;
            break;
    }
    // Check that the sizes are OK based on the type
    switch(NV_ATTRIBUTES_TO_TYPE(attributes))
    {
        case TPM_NT_ORDINARY:
            // Can't exceed the allowed size for the implementation
            if(in->publicInfo.nvPublic.dataSize > MAX_NV_INDEX_SIZE)
                return TPM_RCS_SIZE + RC_NV_DefineSpace_publicInfo;
            break;
        case TPM_NT_EXTEND:
            if(in->publicInfo.nvPublic.dataSize != nameSize)
                return TPM_RCS_SIZE + RC_NV_DefineSpace_publicInfo;
            break;
        default:
            // Everything else needs a size of 8
            if(in->publicInfo.nvPublic.dataSize != 8)
                return TPM_RCS_SIZE + RC_NV_DefineSpace_publicInfo;
            break;
    }
    // Handle other specifics
    switch(NV_ATTRIBUTES_TO_TYPE(attributes))
    {
        case TPM_NT_COUNTER:
            // Counter can't have TPMA_NV_CLEAR_STCLEAR SET (don't clear counters)
            if(IsNv_TPMA_NV_CLEAR_STCLEAR(attributes))
                return TPM_RCS_ATTRIBUTES + RC_NV_DefineSpace_publicInfo;
            break;
#ifdef TPM_NT_PIN_FAIL
        case TPM_NT_PIN_FAIL:
            // NV_NO_DA must be SET and AUTHWRITE must be CLEAR
            // NOTE: As with a PIN_PASS index, the authValue of the index is not
            // available until the index is written. If AUTHWRITE is the only way to
            // write then index, it could never be written. Rather than go through
            // all of the other possible ways to write the Index, it is simply
            // prohibited to write the index with the authValue. Other checks
            // below will insure that there seems to be a way to write the index
            // (i.e., with platform authorization , owner authorization,
            // or with policyAuth.)
            // It is not allowed to create a PIN Index that can't be modified.
            if(!IsNv_TPMA_NV_NO_DA(attributes))
                return TPM_RCS_ATTRIBUTES + RC_NV_DefineSpace_publicInfo;
#endif
#ifdef TPM_NT_PIN_PASS
        case TPM_NT_PIN_PASS:
            // AUTHWRITE must be CLEAR (see note above to TPM_NT_PIN_FAIL)
            if(IsNv_TPMA_NV_AUTHWRITE(attributes)
               || IsNv_TPMA_NV_GLOBALLOCK(attributes)
               || IsNv_TPMA_NV_WRITEDEFINE(attributes))
                return TPM_RCS_ATTRIBUTES + RC_NV_DefineSpace_publicInfo;
#endif  // this comes before break because PIN_FAIL falls through
            break;
        default:
            break;
    }

    // Locks may not be SET and written cannot be SET
    if(IsNv_TPMA_NV_WRITTEN(attributes)
       || IsNv_TPMA_NV_WRITELOCKED(attributes)
       || IsNv_TPMA_NV_READLOCKED(attributes))
        return TPM_RCS_ATTRIBUTES + RC_NV_DefineSpace_publicInfo;

    // There must be a way to read the index.
    if(!IsNv_TPMA_NV_OWNERREAD(attributes)
       && !IsNv_TPMA_NV_PPREAD(attributes)
       && !IsNv_TPMA_NV_AUTHREAD(attributes)
       && !IsNv_TPMA_NV_POLICYREAD(attributes))
        return TPM_RCS_ATTRIBUTES + RC_NV_DefineSpace_publicInfo;

    // There must be a way to write the index
    if(!IsNv_TPMA_NV_OWNERWRITE(attributes)
       && !IsNv_TPMA_NV_PPWRITE(attributes)
       && !IsNv_TPMA_NV_AUTHWRITE(attributes)
       && !IsNv_TPMA_NV_POLICYWRITE(attributes))
        return TPM_RCS_ATTRIBUTES + RC_NV_DefineSpace_publicInfo;

    // An index with TPMA_NV_CLEAR_STCLEAR can't have TPMA_NV_WRITEDEFINE SET
    if(IsNv_TPMA_NV_CLEAR_STCLEAR(attributes)
       &&  IsNv_TPMA_NV_WRITEDEFINE(attributes))
        return TPM_RCS_ATTRIBUTES + RC_NV_DefineSpace_publicInfo;

    // Make sure that the creator of the index can delete the index
    if((IsNv_TPMA_NV_PLATFORMCREATE(attributes)
        && in->authHandle == TPM_RH_OWNER)
       || (!IsNv_TPMA_NV_PLATFORMCREATE(attributes)
           && in->authHandle == TPM_RH_PLATFORM))
        return TPM_RCS_ATTRIBUTES + RC_NV_DefineSpace_authHandle;

    // If TPMA_NV_POLICY_DELETE is SET, then the index must be defined by
    // the platform
    if(IsNv_TPMA_NV_POLICY_DELETE(attributes)
       &&  TPM_RH_PLATFORM != in->authHandle)
        return TPM_RCS_ATTRIBUTES + RC_NV_DefineSpace_publicInfo;

    // Make sure that the TPMA_NV_WRITEALL is not set if the index size is larger
    // than the allowed NV buffer size.
    if(in->publicInfo.nvPublic.dataSize > MAX_NV_BUFFER_SIZE
       &&  IsNv_TPMA_NV_WRITEALL(attributes))
        return TPM_RCS_SIZE + RC_NV_DefineSpace_publicInfo;

    // And finally, see if the index is already defined.
    if(NvIndexIsDefined(in->publicInfo.nvPublic.nvIndex))
        return TPM_RC_NV_DEFINED;

// Internal Data Update
    // define the space.  A TPM_RC_NV_SPACE error may be returned at this point
    return NvDefineIndex(&in->publicInfo.nvPublic, &in->auth);
}

#endif // CC_NV_DefineSpace