/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_SelfTest // Command must be defined
#ifndef _SelfTest_H
#define _SelfTest_H

// Input structure definition
typedef struct {
    TPMI_YES_NO    fullTest;
} SelfTest_In;

// Response code modifiers
#define    RC_SelfTest_fullTest    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_SelfTest(
    SelfTest_In *in
);


#endif  // _SelfTest_H
#endif  // TPM_CC_SelfTest
