/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
/*
    This clause contains the functions used for emulation of functions
    performed by the QSEE
*/


//** Includes
#include "Tpm.h"

#if defined CC_VENDOR_QC_Certify && TPM_CC_VENDOR_QC_Certify == YES
#include "QSEE.h"

uint8_t *
SetToString(
    BYTE *to, 
    const char 
    *string, 
    uint32_t toSize)
{
    uint8_t     *last;
    for(;*string && (toSize > 1); toSize--) 
    {
        *to++ = *string++;
    }
    last = to;
    for(; toSize; toSize--)
    {
        *to++ = 0;

    }
    return last;
}

//*** macchiato_sign_service_data()
int32_t
macchiato_sign_service_data(
    buffer_t            serviceData,
    buffer_t            signedServiceData,
    uint32_t            *pu32SignedServiceDataLength
    )
{
typedef struct  {
    uint32_t            protocolVersion;
    uint32_t            messageId;
    uint32_t            messageLength;
    uint64_t            chipId;
    uint32_t            errorCode;
    uint8_t             signature[256];
    uint8_t             signingKey[256];
    uint8_t             decryptionKey[256];
    uint8_t             fTpmHash[32];
    uint32_t            numServiceId;
    uint16_t            serviceId[1];
    uint8_t             challenge[16];
    uint8_t             response[16];
    uint8_t             toSign[1];
    } certifyMessage;
    
    certifyMessage      *mess = (certifyMessage *)signedServiceData.pu8buffer;
    HASH_STATE          hashState;
    uint8_t             hash[32];
    uint8_t             *pu8Line;
    uint8_t             *pu8Hash = hash;
    int32_t             i;

    *pu32SignedServiceDataLength = sizeof(certifyMessage) + serviceData.u32Len - 1;
    if(*pu32SignedServiceDataLength > signedServiceData.u32Len)
    {
        *pu32SignedServiceDataLength = 0;
        return -1;
    }
    mess->messageLength = *pu32SignedServiceDataLength;     
    mess->protocolVersion = 0;
    mess->messageId = 3;
  
    
    SetToString((BYTE *)(&mess->chipId),  "Chip ID", sizeof(mess->chipId));
    mess->errorCode = 0;
    SetToString(mess->signingKey, 
                "This is the public signing key", 
                sizeof(mess->signingKey));
    SetToString(mess->decryptionKey, 
                "This is the public decryption key",
                sizeof(mess->decryptionKey));
    SetToString(mess->fTpmHash, 
                "This is the hash of the fTPM code",
                sizeof(mess->fTpmHash));
    SetToString(mess->challenge, "Challenge", sizeof(mess->challenge));
    SetToString(mess->response, "Response", sizeof(mess->response));
    MemoryCopy(mess->toSign, 
               serviceData.pu8buffer, 
               serviceData.u32Len, 
               signedServiceData.u32Len);

   CryptHashStart(&hashState, TPM_ALG_SHA256);

    // hash all the data
    CryptDigestUpdate(&hashState, 
                      mess->messageLength - offsetof(certifyMessage, signingKey), 
                      &mess->signingKey[0]);

    // complete the operation
    CryptHashEnd(&hashState, sizeof(hash), hash);

    // If we were going to sign this with an RSA key we would do it here, but, for
    // the purposes of prototyping, just put the digest in the signature field
    MemorySet((BYTE *)mess->signature, 0, sizeof(mess->signature));
    pu8Line = SetToString(mess->signature, "This is a hash of the data: ", 
                sizeof(mess->signature));
    for(i = 0; i < 32; i++)
    {
        sprintf((char *)pu8Line, "%02x", *pu8Hash++);
        pu8Line += 2;
    }
    return 0;
}

#endif