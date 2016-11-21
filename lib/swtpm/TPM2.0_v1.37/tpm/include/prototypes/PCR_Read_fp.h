/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PCR_Read // Command must be defined
#ifndef _PCR_Read_H
#define _PCR_Read_H

// Input structure definition
typedef struct {
    TPML_PCR_SELECTION    pcrSelectionIn;
} PCR_Read_In;

// Output structure definition
typedef struct {
    UINT32                pcrUpdateCounter;
    TPML_PCR_SELECTION    pcrSelectionOut;
    TPML_DIGEST           pcrValues;
} PCR_Read_Out;

// Response code modifiers
#define    RC_PCR_Read_pcrSelectionIn    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PCR_Read(
    PCR_Read_In *in,
    PCR_Read_Out *out
);


#endif  // _PCR_Read_H
#endif  // TPM_CC_PCR_Read
