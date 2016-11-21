/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_ZGen_2Phase // Command must be defined
#ifndef _ZGen_2Phase_H
#define _ZGen_2Phase_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT           keyA;
    TPM2B_ECC_POINT          inQsB;
    TPM2B_ECC_POINT          inQeB;
    TPMI_ECC_KEY_EXCHANGE    inScheme;
    UINT16                   counter;
} ZGen_2Phase_In;

// Output structure definition
typedef struct {
    TPM2B_ECC_POINT          outZ1;
    TPM2B_ECC_POINT          outZ2;
} ZGen_2Phase_Out;

// Response code modifiers
#define    RC_ZGen_2Phase_keyA        (TPM_RC_H + TPM_RC_1)
#define    RC_ZGen_2Phase_inQsB       (TPM_RC_P + TPM_RC_1)
#define    RC_ZGen_2Phase_inQeB       (TPM_RC_P + TPM_RC_2)
#define    RC_ZGen_2Phase_inScheme    (TPM_RC_P + TPM_RC_3)
#define    RC_ZGen_2Phase_counter     (TPM_RC_P + TPM_RC_4)

// Function prototype
TPM_RC
TPM2_ZGen_2Phase(
    ZGen_2Phase_In *in,
    ZGen_2Phase_Out *out
);


#endif  // _ZGen_2Phase_H
#endif  // TPM_CC_ZGen_2Phase
