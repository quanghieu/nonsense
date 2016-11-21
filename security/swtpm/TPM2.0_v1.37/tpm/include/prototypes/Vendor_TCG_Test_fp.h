/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_Vendor_TCG_Test // Command must be defined
#ifndef _Vendor_TCG_Test_H
#define _Vendor_TCG_Test_H

// Input structure definition
typedef struct {
    TPM2B_DATA    inputData;
} Vendor_TCG_Test_In;

// Output structure definition
typedef struct {
    TPM2B_DATA    outputData;
} Vendor_TCG_Test_Out;

// Response code modifiers
#define    RC_Vendor_TCG_Test_inputData    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_Vendor_TCG_Test(
    Vendor_TCG_Test_In *in,
    Vendor_TCG_Test_Out *out
);


#endif  // _Vendor_TCG_Test_H
#endif  // TPM_CC_Vendor_TCG_Test
