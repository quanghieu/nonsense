/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_PolicyTicket // Command must be defined
#ifndef _PolicyTicket_H
#define _PolicyTicket_H

// Input structure definition
typedef struct {
    TPMI_SH_POLICY    policySession;
    TPM2B_TIMEOUT     timeout;
    TPM2B_DIGEST      cpHashA;
    TPM2B_NONCE       policyRef;
    TPM2B_NAME        authName;
    TPMT_TK_AUTH      ticket;
} PolicyTicket_In;

// Response code modifiers
#define    RC_PolicyTicket_policySession    (TPM_RC_H + TPM_RC_1)
#define    RC_PolicyTicket_timeout          (TPM_RC_P + TPM_RC_1)
#define    RC_PolicyTicket_cpHashA          (TPM_RC_P + TPM_RC_2)
#define    RC_PolicyTicket_policyRef        (TPM_RC_P + TPM_RC_3)
#define    RC_PolicyTicket_authName         (TPM_RC_P + TPM_RC_4)
#define    RC_PolicyTicket_ticket           (TPM_RC_P + TPM_RC_5)

// Function prototype
TPM_RC
TPM2_PolicyTicket(
    PolicyTicket_In *in
);


#endif  // _PolicyTicket_H
#endif  // TPM_CC_PolicyTicket
