/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_IncrementalSelfTest // Command must be defined
#ifndef _IncrementalSelfTest_H
#define _IncrementalSelfTest_H

// Input structure definition
typedef struct {
    TPML_ALG    toTest;
} IncrementalSelfTest_In;

// Output structure definition
typedef struct {
    TPML_ALG    toDoList;
} IncrementalSelfTest_Out;

// Response code modifiers
#define    RC_IncrementalSelfTest_toTest    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_IncrementalSelfTest(
    IncrementalSelfTest_In *in,
    IncrementalSelfTest_Out *out
);


#endif  // _IncrementalSelfTest_H
#endif  // TPM_CC_IncrementalSelfTest
