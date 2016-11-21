/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_Unseal // Command must be defined
#ifndef _Unseal_H
#define _Unseal_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT          itemHandle;
} Unseal_In;

// Output structure definition
typedef struct {
    TPM2B_SENSITIVE_DATA    outData;
} Unseal_Out;

// Response code modifiers
#define    RC_Unseal_itemHandle     (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_Unseal(
    Unseal_In *in,
    Unseal_Out *out
);


#endif  // _Unseal_H
#endif  // TPM_CC_Unseal
