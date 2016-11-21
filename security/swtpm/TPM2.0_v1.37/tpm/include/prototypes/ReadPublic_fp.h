/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_ReadPublic // Command must be defined
#ifndef _ReadPublic_H
#define _ReadPublic_H

// Input structure definition
typedef struct {
    TPMI_DH_OBJECT    objectHandle;
} ReadPublic_In;

// Output structure definition
typedef struct {
    TPM2B_PUBLIC      outPublic;
    TPM2B_NAME        name;
    TPM2B_NAME        qualifiedName;
} ReadPublic_Out;

// Response code modifiers
#define    RC_ReadPublic_objectHandle    (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_ReadPublic(
    ReadPublic_In *in,
    ReadPublic_Out *out
);


#endif  // _ReadPublic_H
#endif  // TPM_CC_ReadPublic
