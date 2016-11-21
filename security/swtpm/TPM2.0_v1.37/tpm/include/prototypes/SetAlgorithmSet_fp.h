/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_SetAlgorithmSet // Command must be defined
#ifndef _SetAlgorithmSet_H
#define _SetAlgorithmSet_H

// Input structure definition
typedef struct {
    TPMI_RH_PLATFORM    authHandle;
    UINT32              algorithmSet;
} SetAlgorithmSet_In;

// Response code modifiers
#define    RC_SetAlgorithmSet_authHandle      (TPM_RC_H + TPM_RC_1)
#define    RC_SetAlgorithmSet_algorithmSet    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_SetAlgorithmSet(
    SetAlgorithmSet_In *in
);


#endif  // _SetAlgorithmSet_H
#endif  // TPM_CC_SetAlgorithmSet
