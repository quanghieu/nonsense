/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_ECC_Parameters // Command must be defined
#ifndef _ECC_Parameters_H
#define _ECC_Parameters_H

// Input structure definition
typedef struct {
    TPMI_ECC_CURVE               curveID;
} ECC_Parameters_In;

// Output structure definition
typedef struct {
    TPMS_ALGORITHM_DETAIL_ECC    parameters;
} ECC_Parameters_Out;

// Response code modifiers
#define    RC_ECC_Parameters_curveID    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_ECC_Parameters(
    ECC_Parameters_In *in,
    ECC_Parameters_Out *out
);


#endif  // _ECC_Parameters_H
#endif  // TPM_CC_ECC_Parameters
