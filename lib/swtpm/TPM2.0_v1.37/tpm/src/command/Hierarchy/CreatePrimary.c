/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include "Tpm.h"
#include "CreatePrimary_fp.h"

#ifdef TPM_CC_CreatePrimary  // Conditional expansion of this file

/*(See part 3 specification)
// Creates a primary or temporary object from a primary seed.
*/
// return type: TPM_RC
//   TPM_RC_ATTRIBUTES      'sensitiveDataOrigin' is CLEAR when 'sensitive.data' is
//                          an Empty Buffer, or is SET when 'sensitive.data' is not
//                          empty;
//                          'fixedTPM', 'fixedParent', or 'encryptedDuplication'
//                          attributes are inconsistent between themselves or with
//                          those of the parent object;
//                          inconsistent 'restricted', 'decrypt' and 'sign'
//                          attributes;
//                          attempt to inject sensitive data for an asymmetric key;
//                          attempt to create a symmetric cipher key that is not
//                          a decryption key
//   TPM_RC_KDF             incorrect KDF specified for decrypting keyed hash object
//   TPM_RC_KEY             a provided symmetric key value is not allowed
//   TPM_RC_OBJECT_MEMORY   there is no free slot for the object
//   TPM_RC_SCHEME          inconsistent attributes 'decrypt', 'sign', 'restricted'
//                          and key's scheme ID; or hash algorithm is inconsistent
//                          with the scheme ID for keyed hash object
//   TPM_RC_SIZE            size of public authorization policy or sensitive
//                          authorization value does not match digest size of the
//                          name algorithm; or sensitive data size for the keyed
//                          hash object is larger than is allowed for the scheme
//   TPM_RC_SYMMETRIC       a storage key with no symmetric algorithm specified; or
//                          non-storage key with symmetric algorithm different from
//                          TPM_ALG_NULL
//   TPM_RC_TYPE            unknown object type
TPM_RC
TPM2_CreatePrimary(
    CreatePrimary_In    *in,            // IN: input parameter list
    CreatePrimary_Out   *out            // OUT: output parameter list
    )
{
// Local variables
    TPM_RC               result = TPM_RC_SUCCESS;
    TPMT_PUBLIC         *publicArea;
    DRBG_STATE           rand;
    OBJECT              *newObject;
    TPM2B_NAME           name;

// Input Validation
    // Will need a place to put the result
    newObject = FindEmptyObjectSlot(&out->objectHandle);
    if(newObject == NULL)
        return TPM_RC_OBJECT_MEMORY;
    // Get the address of the public area in the new object 
    // (this is just to save typing)
    publicArea = &newObject->publicArea;

    *publicArea = in->inPublic.publicArea;

    // Check attributes in input public area. CreateChecks() checks the things that
    // are unique to creation and then validates the attributes and values that are
    // common to create and load.
    result = CreateChecks(NULL, publicArea);
    if(result != TPM_RC_SUCCESS)
        return RcSafeAddToResult(result, RC_CreatePrimary_inPublic);

    // Validate the sensitive area values
    if(!AdjustAuthSize(&in->inSensitive.sensitive.userAuth,
                       publicArea->nameAlg))
        return TPM_RCS_SIZE + RC_CreatePrimary_inSensitive;

// Command output
    // Compute the name using out->name as a scratch area (this is not the value
    // that ultimately will be returned, then instantiate the state that will be
    // used as a random number generator during the object creation.
    // The caller does not know the seed values so the actual name does not have
    // to be over the input, it can be over the unmarshaled structure.
    DRBG_InstantiateSeeded(&rand, 
                           &HierarchyGetPrimarySeed(in->primaryHandle)->b, 
                           PRIMARY_OBJECT_CREATION,
                           (TPM2B *)PublicMarshalAndComputeName(publicArea, &name),
                           &in->inSensitive.sensitive.data.b);
    newObject->attributes.primary = SET;
    if(in->primaryHandle == TPM_RH_ENDORSEMENT)
        newObject->attributes.epsHierarchy = SET;

    // Create the primary object.
    result = CryptCreateObject(newObject, &in->inSensitive.sensitive, 
                               (RAND_STATE *)&rand);
    if(result != TPM_RC_SUCCESS)
        return result;

    // Set the publicArea and name from the computed values
    out->outPublic.publicArea = newObject->publicArea;
    out->name = newObject->name;

    // Fill in creation data
    FillInCreationData(in->primaryHandle, publicArea->nameAlg,
                       &in->creationPCR, &in->outsideInfo, &out->creationData,
                       &out->creationHash);

    // Compute creation ticket
    TicketComputeCreation(EntityGetHierarchy(in->primaryHandle), &out->name,
                          &out->creationHash, &out->creationTicket);

    // Set the remaining attributes for a loaded object
    ObjectSetLoadedAttributes(newObject, in->primaryHandle);
    return result;
}

#endif // CC_CreatePrimary