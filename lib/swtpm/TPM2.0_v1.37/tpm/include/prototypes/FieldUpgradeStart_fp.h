/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_FieldUpgradeStart // Command must be defined
#ifndef _FieldUpgradeStart_H
#define _FieldUpgradeStart_H

// Input structure definition
typedef struct {
    TPMI_RH_PLATFORM    authorization;
    TPMI_DH_OBJECT      keyHandle;
    TPM2B_DIGEST        fuDigest;
    TPMT_SIGNATURE      manifestSignature;
} FieldUpgradeStart_In;

// Response code modifiers
#define    RC_FieldUpgradeStart_authorization        (TPM_RC_H + TPM_RC_1)
#define    RC_FieldUpgradeStart_keyHandle            (TPM_RC_H + TPM_RC_2)
#define    RC_FieldUpgradeStart_fuDigest             (TPM_RC_P + TPM_RC_1)
#define    RC_FieldUpgradeStart_manifestSignature    (TPM_RC_P + TPM_RC_2)

// Function prototype
TPM_RC
TPM2_FieldUpgradeStart(
    FieldUpgradeStart_In *in
);


#endif  // _FieldUpgradeStart_H
#endif  // TPM_CC_FieldUpgradeStart
