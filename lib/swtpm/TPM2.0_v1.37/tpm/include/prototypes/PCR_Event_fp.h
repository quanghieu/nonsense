/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PCR_Event // Command must be defined
#ifndef _PCR_Event_H
#define _PCR_Event_H

// Input structure definition
typedef struct {
    TPMI_DH_PCR           pcrHandle;
    TPM2B_EVENT           eventData;
} PCR_Event_In;

// Output structure definition
typedef struct {
    TPML_DIGEST_VALUES    digests;
} PCR_Event_Out;

// Response code modifiers
#define    RC_PCR_Event_pcrHandle     (TPM_RC_H + TPM_RC_1)
#define    RC_PCR_Event_eventData     (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PCR_Event(
    PCR_Event_In *in,
    PCR_Event_Out *out
);


#endif  // _PCR_Event_H
#endif  // TPM_CC_PCR_Event
