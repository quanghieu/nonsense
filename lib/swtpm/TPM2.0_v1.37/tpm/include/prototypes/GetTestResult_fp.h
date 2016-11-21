/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_GetTestResult // Command must be defined
#ifndef _GetTestResult_H
#define _GetTestResult_H

// Output structure definition
typedef struct {
    TPM2B_MAX_BUFFER    outData;
    TPM_RC              testResult;
} GetTestResult_Out;


// Function prototype
TPM_RC
TPM2_GetTestResult(
    GetTestResult_Out *out
);


#endif  // _GetTestResult_H
#endif  // TPM_CC_GetTestResult
