/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyPhysicalPresence // Command must be defined
#ifndef _PolicyPhysicalPresence_H
#define _PolicyPhysicalPresence_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY    policySession;
} PolicyPhysicalPresence_In;

// Response code modifiers
#define    RC_PolicyPhysicalPresence_policySession    (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_PolicyPhysicalPresence(
    PolicyPhysicalPresence_In *in
);


#endif  // _PolicyPhysicalPresence_H
#endif  // TPM_CC_PolicyPhysicalPresence
