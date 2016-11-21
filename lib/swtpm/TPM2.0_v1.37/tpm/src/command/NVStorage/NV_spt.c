/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Includes
#include "Tpm.h"
#include "NV_spt_fp.h"

//** Functions

//*** NvReadAccessChecks()
// Common routine for validating a read
// Used by TPM2_NV_Read, TPM2_NV_ReadLock and TPM2_PolicyNV
// return type: TPM_RC
//      TPM_RC_NV_AUTHORIZATION     autHandle is not allowed to authorize read
//                                  of the index
//      TPM_RC_NV_LOCKED            Read locked
//      TPM_RC_NV_UNINITIALIZED     Try to read an uninitialized index
//
TPM_RC
NvReadAccessChecks(
    TPM_HANDLE       authHandle,    // IN: the handle that provided the
                                    //     authorization
    TPM_HANDLE       nvHandle,      // IN: the handle of the NV index to be read
    TPMA_NV          attributes     // IN: the attributes of 'nvHandle'
    )
{
    // If data is read locked, returns an error
   if(IsNv_TPMA_NV_READLOCKED(attributes))
       return TPM_RC_NV_LOCKED;
    // If the authorization was provided by the owner or platform, then check
    // that the attributes allow the read.  If the authorization handle
    // is the same as the index, then the checks were made when the authorization
    // was checked..
    if(authHandle == TPM_RH_OWNER)
    {
        // If Owner provided authorization then ONWERWRITE must be SET
        if(!IsNv_TPMA_NV_OWNERREAD(attributes))
            return TPM_RC_NV_AUTHORIZATION;
    }
    else if(authHandle == TPM_RH_PLATFORM)
    {
        // If Platform provided authorization then PPWRITE must be SET
        if(!IsNv_TPMA_NV_PPREAD(attributes))
            return TPM_RC_NV_AUTHORIZATION;
    }
    // If neither Owner nor Platform provided authorization, make sure that it was
    // provided by this index.
    else if(authHandle != nvHandle)
        return TPM_RC_NV_AUTHORIZATION;

// If the index has not been written, then the value cannot be read
// NOTE: This has to come after other access checks to make sure that
// the proper authorization is given to TPM2_NV_ReadLock()
    if(!IsNv_TPMA_NV_WRITTEN(attributes))
        return TPM_RC_NV_UNINITIALIZED;

    return TPM_RC_SUCCESS;
}

//*** NvWriteAccessChecks()
// Common routine for validating a write
// Used by TPM2_NV_Write, TPM2_NV_Increment, TPM2_SetBits, and TPM2_NV_WriteLock
// return type: TPM_RC
//      TPM_RC_NV_AUTHORIZATION     Authorization fails
//      TPM_RC_NV_LOCKED            Write locked
//
TPM_RC
NvWriteAccessChecks(
    TPM_HANDLE       authHandle,    // IN: the handle that provided the
                                    //     authorization
    TPM_HANDLE       nvHandle,      // IN: the handle of the NV index to be written
    TPMA_NV          attributes     // IN: the attributes of 'nvHandle'
    )
{
    // If data is write locked, returns an error
    if(IsNv_TPMA_NV_WRITELOCKED(attributes))
        return TPM_RC_NV_LOCKED;
    // If the authorization was provided by the owner or platform, then check
    // that the attributes allow the write.  If the authorization handle
    // is the same as the index, then the checks were made when the authorization
    // was checked..
    if(authHandle == TPM_RH_OWNER)
    {
        // If Owner provided authorization then ONWERWRITE must be SET
        if(!IsNv_TPMA_NV_OWNERWRITE(attributes))
            return TPM_RC_NV_AUTHORIZATION;
    }
    else if(authHandle == TPM_RH_PLATFORM)
    {
        // If Platform provided authorization then PPWRITE must be SET
        if(!IsNv_TPMA_NV_PPWRITE(attributes))
            return TPM_RC_NV_AUTHORIZATION;
    }
    // If neither Owner nor Platform provided authorization, make sure that it was
    // provided by this index.
    else if(authHandle != nvHandle)
        return TPM_RC_NV_AUTHORIZATION;

    return TPM_RC_SUCCESS;
}

//*** NvClearOrderly()
// This function is used to cause gp.orderlyState to be cleared to the
// non-orderly state.
TPM_RC
NvClearOrderly(
    void
    )
{
    if(gp.orderlyState < SU_DA_USED_VALUE)
        RETURN_IF_NV_IS_NOT_AVAILABLE;
    g_clearOrderly = TRUE;
    return TPM_RC_SUCCESS;
}

//*** NvIsPinPassIndex()
// Function to check to see if an NV index is a PIN Pass Index
// return type: BOOL
//  TRUE    is pin pass
//  FALSE   is not pin pass
BOOL
NvIsPinPassIndex(
    TPM_HANDLE          index       // IN: Handle to check
    )
{
    if(HandleGetType(index) == TPM_HT_NV_INDEX)
    {
        NV_INDEX                *nvIndex = NvGetIndexInfo(index, NULL);

        return IsNvPinPassIndex(nvIndex->publicArea.attributes);
    }
    return FALSE;
}
