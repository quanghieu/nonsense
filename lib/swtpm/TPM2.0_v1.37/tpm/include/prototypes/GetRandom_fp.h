/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_GetRandom // Command must be defined
#ifndef _GetRandom_H
#define _GetRandom_H

// Input structure definition
typedef struct {
    UINT16          bytesRequested;
} GetRandom_In;

// Output structure definition
typedef struct {
    TPM2B_DIGEST    randomBytes;
} GetRandom_Out;

// Response code modifiers
#define    RC_GetRandom_bytesRequested    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_GetRandom(
    GetRandom_In *in,
    GetRandom_Out *out
);


#endif  // _GetRandom_H
#endif  // TPM_CC_GetRandom
