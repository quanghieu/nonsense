/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Description
// The functions in this file are used for accessing properties for handles of
// various types. Functions in other files require handles of a specific
// type but the functions in this file allow use of any handle type.

//** Includes

#include "Tpm.h"

//** Functions
//*** EntityGetLoadStatus()
// This function will check that all the handles access loaded entities.
// return type: TPM_RC
//      TPM_RC_HANDLE           handle type does not match
//      TPM_RC_REFERENCE_Hx     entity is not present
//      TPM_RC_HIERARCHY        entity belongs to a disabled hierarchy
//      TPM_RC_OBJECT_MEMORY    handle is an evict object but there is no
//                               space to load it to RAM
TPM_RC
EntityGetLoadStatus(
    COMMAND         *command        // IN/OUT: command parsing structure
    )
{
    UINT32               i;
    TPM_RC               result = TPM_RC_SUCCESS;
//
    for(i = 0; i < command->handleNum; i++)
    {
        TPM_HANDLE      handle = command->handles[i];
        switch(HandleGetType(handle))
        {
            // For handles associated with hierarchies, the entity is present
            // only if the associated enable is SET.
            case TPM_HT_PERMANENT:
                switch(handle)
                {
                    case TPM_RH_OWNER:
                        if(!gc.shEnable)
                            result = TPM_RC_HIERARCHY;
                        break;

#ifdef  VENDOR_PERMANENT
                    case VENDOR_PERMANENT:
#endif
                    case TPM_RH_ENDORSEMENT:
                        if(!gc.ehEnable)
                            result = TPM_RC_HIERARCHY;
                        break;
                    case TPM_RH_PLATFORM:
                        if(!g_phEnable)
                            result = TPM_RC_HIERARCHY;
                        break;
                        // null handle, PW session handle and lockout
                        // handle are always available
                    case TPM_RH_NULL:
                    case TPM_RS_PW:
                        // Need to be careful for lockout. Lockout is always available
                        // for policy checks but not always available when authValue
                        // is being checked.
                    case TPM_RH_LOCKOUT:
                        break;
                    default:
                        // handling of the manufacture_specific handles
                        if(((TPM_RH)handle >= TPM_RH_AUTH_00)
                           && ((TPM_RH)handle <= TPM_RH_AUTH_FF))
                           // use the value that would have been returned from
                           // unmarshaling if it did the handle filtering
                            result = TPM_RC_VALUE;
                        else
                            FAIL(FATAL_ERROR_INTERNAL);
                        break;
                }
                break;
            case TPM_HT_TRANSIENT:
                // For a transient object, check if the handle is associated
                // with a loaded object.
                if(!IsObjectPresent(handle))
                    result = TPM_RC_REFERENCE_H0;
                break;
            case TPM_HT_PERSISTENT:
                // Persistent object
                // Copy the persistent object to RAM and replace the handle with the
                // handle of the assigned slot.  A TPM_RC_OBJECT_MEMORY,
                // TPM_RC_HIERARCHY or TPM_RC_REFERENCE_H0 error may be returned by
                // ObjectLoadEvict()
                result = ObjectLoadEvict(&command->handles[i], command->index);
                break;
            case TPM_HT_HMAC_SESSION:
                // For an HMAC session, see if the session is loaded
                // and if the session in the session slot is actually
                // an HMAC session.
                if(SessionIsLoaded(handle))
                {
                    SESSION             *session;
                    session = SessionGet(handle);
                    // Check if the session is a HMAC session
                    if(session->attributes.isPolicy == SET)
                        result = TPM_RC_HANDLE;
                }
                else
                    result = TPM_RC_REFERENCE_H0;
                break;
            case TPM_HT_POLICY_SESSION:
                // For a policy session, see if the session is loaded
                // and if the session in the session slot is actually
                // a policy session.
                if(SessionIsLoaded(handle))
                {
                    SESSION             *session;
                    session = SessionGet(handle);
                    // Check if the session is a policy session
                    if(session->attributes.isPolicy == CLEAR)
                        result = TPM_RC_HANDLE;
                }
                else
                    result = TPM_RC_REFERENCE_H0;
                break;
            case TPM_HT_NV_INDEX:
                // For an NV Index, use the platform-specific routine
                // to search the IN Index space.
                result = NvIndexIsAccessible(handle);
                break;
            case TPM_HT_PCR:
                // Any PCR handle that is unmarshaled successfully referenced
                // a PCR that is defined.
                break;
            default:
                // Any other handle type is a defect in the unmarshaling code.
                FAIL(FATAL_ERROR_INTERNAL);
                break;
        }
        if(result != TPM_RC_SUCCESS)
        {
            if(result == TPM_RC_REFERENCE_H0)
                result = result + i;
            else
                result = RcSafeAddToResult(result, TPM_RC_H + g_rcIndex[i]);
            break;
        }
    }
    return result;
}

//*** EntityGetAuthValue()
// This function is used to access the 'authValue' associated with a handle.
// This function assumes that the handle references an entity that is accessible
// and the handle is not for a persistent objects. That is EntityGetLoadStatus()
// should have been called. Also, the accessibility of the authValue should have
// been verified by IsAuthValueAvailable().
//
// This function copies the authorization value of the entity to 'auth'.
// Return Type: UINT16
//  count        number of bytes in the authValue with 0's stripped
UINT16
EntityGetAuthValue(
    TPMI_DH_ENTITY   handle,        // IN: handle of entity
    TPM2B_AUTH      *auth           // OUT: authValue of the entity
    )
{
    TPM2B_AUTH      *pAuth = NULL;

    auth->t.size = 0;

    switch(HandleGetType(handle))
    {
        case TPM_HT_PERMANENT:
        {
            switch(handle)
            {
                case TPM_RH_OWNER:
                    // ownerAuth for TPM_RH_OWNER
                    pAuth = &gp.ownerAuth;
                    break;
                case TPM_RH_ENDORSEMENT:
                    // endorsementAuth for TPM_RH_ENDORSEMENT
                    pAuth = &gp.endorsementAuth;
                    break;
                case TPM_RH_PLATFORM:
                    // platformAuth for TPM_RH_PLATFORM
                    pAuth = &gc.platformAuth;
                    break;
                case TPM_RH_LOCKOUT:
                    // lockoutAuth for TPM_RH_LOCKOUT
                    pAuth = &gp.lockoutAuth;
                    break;
                case TPM_RH_NULL:
                    // nullAuth for TPM_RH_NULL. Return 0 directly here
                    return 0;
                    break;
#ifdef  VENDOR_PERMANENT
                case VENDOR_PERMANENT:
                    // vendor authorization value
                    pAauth = &g_platformUniqueDetails;
#endif
                default:
                    // If any other permanent handle is present it is
                    // a code defect.
                    FAIL(FATAL_ERROR_INTERNAL);
                    break;
            }
            break;
        }
        case TPM_HT_TRANSIENT:
            // authValue for an object
            // A persistent object would have been copied into RAM
            // and would have an transient object handle here.
        {
            OBJECT          *object;

            object = HandleToObject(handle);
            // special handling if this is a sequence object
            if(ObjectIsSequence(object))
            {
                pAuth = &((HASH_OBJECT *)object)->auth;
            }
            else
            {
                // Authorization is available only when the private portion of
                // the object is loaded.  The check should be made before
                // this function is called
                pAssert(object->attributes.publicOnly == CLEAR);
                pAuth = &object->sensitive.authValue;
            }
        }
        break;
        case TPM_HT_NV_INDEX:
            // authValue for an NV index
        {
            NV_INDEX        *nvIndex = NvGetIndexInfo(handle, NULL);
            pAssert(nvIndex != NULL);
            pAuth = &nvIndex->authValue;
        }
        break;
        case TPM_HT_PCR:
            // authValue for PCR
            pAuth = PCRGetAuthValue(handle);
            break;
        default:
            // If any other handle type is present here, then there is a defect
            // in the unmarshaling code.
            FAIL(FATAL_ERROR_INTERNAL);
            break;
    }
    // Copy the authValue
    MemoryCopy2B(&auth->b, &pAuth->b, sizeof(auth->t.buffer));
    MemoryRemoveTrailingZeros(auth);
    return auth->t.size;
}

//*** EntityGetAuthPolicy()
// This function is used to access the 'authPolicy' associated with a handle.
// This function assumes that the handle references an entity that is accessible
// and the handle is not for a persistent objects. That is EntityGetLoadStatus()
// should have been called. Also, the accessibility of the authPolicy should have
// been verified by IsAuthPolicyAvailable().
//
// This function copies the authorization policy of the entity to 'authPolicy'.
//
//  The return value is the hash algorithm for the policy.
TPMI_ALG_HASH
EntityGetAuthPolicy(
    TPMI_DH_ENTITY   handle,        // IN: handle of entity
    TPM2B_DIGEST    *authPolicy     // OUT: authPolicy of the entity
    )
{
    TPMI_ALG_HASH       hashAlg = TPM_ALG_NULL;
    authPolicy->t.size = 0;

    switch(HandleGetType(handle))
    {
        case TPM_HT_PERMANENT:
            switch(handle)
            {
                case TPM_RH_OWNER:
                    // ownerPolicy for TPM_RH_OWNER
                    *authPolicy = gp.ownerPolicy;
                    hashAlg = gp.ownerAlg;
                    break;
                case TPM_RH_ENDORSEMENT:
                    // endorsementPolicy for TPM_RH_ENDORSEMENT
                    *authPolicy = gp.endorsementPolicy;
                    hashAlg = gp.endorsementAlg;
                    break;
                case TPM_RH_PLATFORM:
                    // platformPolicy for TPM_RH_PLATFORM
                    *authPolicy = gc.platformPolicy;
                    hashAlg = gc.platformAlg;
                    break;
                case TPM_RH_LOCKOUT:
                    // lockoutPolicy for TPM_RH_LOCKOUT
                    *authPolicy = gp.lockoutPolicy;
                    hashAlg = gp.lockoutAlg;
                    break;
                default:
                    return TPM_ALG_ERROR;
                    break;
            }
            break;
        case TPM_HT_TRANSIENT:
            // authPolicy for an object
        {
            OBJECT *object = HandleToObject(handle);
            *authPolicy = object->publicArea.authPolicy;
            hashAlg = object->publicArea.nameAlg;
        }
        break;
        case TPM_HT_NV_INDEX:
            // authPolicy for a NV index
        {
            NV_INDEX        *nvIndex = NvGetIndexInfo(handle, NULL);
            pAssert(nvIndex != 0);
            *authPolicy = nvIndex->publicArea.authPolicy;
            hashAlg = nvIndex->publicArea.nameAlg;
        }
        break;
        case TPM_HT_PCR:
            // authPolicy for a PCR
            hashAlg = PCRGetAuthPolicy(handle, authPolicy);
            break;
        default:
            // If any other handle type is present it is a code defect.
            FAIL(FATAL_ERROR_INTERNAL);
            break;
    }
    return hashAlg;
}

//*** EntityGetName()
// This function returns the Name associated with a handle.
TPM2B_NAME *
EntityGetName(
    TPMI_DH_ENTITY   handle,        // IN: handle of entity
    TPM2B_NAME      *name           // OUT: name of entity
    )
{
    switch(HandleGetType(handle))
    {
        case TPM_HT_TRANSIENT:
        {
            // Name for an object
            OBJECT      *object = HandleToObject(handle);
            // an object with no nameAlg has no name
            if(object->publicArea.nameAlg == TPM_ALG_NULL)
                name->b.size = 0;
            else
                *name = object->name;
            break;
        }
        case TPM_HT_NV_INDEX:
            // Name for a NV index
            NvGetNameByIndexHandle(handle, name);
            break;
        default:
            // For all other types, the handle is the Name
            name->t.size = sizeof(TPM_HANDLE);
            UINT32_TO_BYTE_ARRAY(handle, name->t.name);
            break;
    }
    return name;
}

//*** EntityGetHierarchy()
// This function returns the hierarchy handle associated with an entity.
//      1. A handle that is a hierarchy handle is associated with itself.
//      2. An NV index belongs to TPM_RH_PLATFORM if TPMA_NV_PLATFORMCREATE,
//         is SET, otherwise it belongs to TPM_RH_OWNER
//      3. An object handle belongs to its hierarchy.
// All other handles belong to the platform hierarchy.
// or an NV Index.
TPMI_RH_HIERARCHY
EntityGetHierarchy(
    TPMI_DH_ENTITY   handle         // IN :handle of entity
    )
{
    TPMI_RH_HIERARCHY       hierarcy = TPM_RH_NULL;

    switch(HandleGetType(handle))
    {
        case TPM_HT_PERMANENT:
            // hierarchy for a permanent handle
            switch(handle)
            {
                case TPM_RH_PLATFORM:
                case TPM_RH_ENDORSEMENT:
                case TPM_RH_NULL:
                    hierarcy = handle;
                    break;
                // all other permanent handles are associated with the owner
                // hierarchy. (should only be TPM_RH_OWNER and TPM_RH_LOCKOUT)
                default:
                    hierarcy = TPM_RH_OWNER;
                    break;
            }
            break;
        case TPM_HT_NV_INDEX:
            // hierarchy for NV index
        {
            NV_INDEX        *nvIndex = NvGetIndexInfo(handle, NULL);
            pAssert(nvIndex != NULL);

            // If only the platform can delete the index, then it is
            // considered to be in the platform hierarchy, otherwise it
            // is in the owner hierarchy.
            if(IsNv_TPMA_NV_PLATFORMCREATE(nvIndex->publicArea.attributes))
                hierarcy = TPM_RH_PLATFORM;
            else
                hierarcy = TPM_RH_OWNER;
        }
        break;
        case TPM_HT_TRANSIENT:
            // hierarchy for an object
        {
            OBJECT          *object;
            object = HandleToObject(handle);
            if(object->attributes.ppsHierarchy)
            {
                hierarcy = TPM_RH_PLATFORM;
            }
            else if(object->attributes.epsHierarchy)
            {
                hierarcy = TPM_RH_ENDORSEMENT;
            }
            else if(object->attributes.spsHierarchy)
            {
                hierarcy = TPM_RH_OWNER;
            }
        }
        break;
        case TPM_HT_PCR:
            hierarcy = TPM_RH_OWNER;
            break;
        default:
            FAIL(FATAL_ERROR_INTERNAL);
            break;
    }
    // this is unreachable but it provides a return value for the default
    // case which makes the complier happy
    return hierarcy;
}