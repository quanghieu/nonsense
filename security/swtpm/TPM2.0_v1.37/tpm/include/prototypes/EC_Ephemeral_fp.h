/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_EC_Ephemeral // Command must be defined
#ifndef _EC_Ephemeral_H
#define _EC_Ephemeral_H

// Input structure definition
typedef struct {
    TPMI_ECC_CURVE     curveID;
} EC_Ephemeral_In;

// Output structure definition
typedef struct {
    TPM2B_ECC_POINT    Q;
    UINT16             counter;
} EC_Ephemeral_Out;

// Response code modifiers
#define    RC_EC_Ephemeral_curveID    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_EC_Ephemeral(
    EC_Ephemeral_In *in,
    EC_Ephemeral_Out *out
);


#endif  // _EC_Ephemeral_H
#endif  // TPM_CC_EC_Ephemeral
