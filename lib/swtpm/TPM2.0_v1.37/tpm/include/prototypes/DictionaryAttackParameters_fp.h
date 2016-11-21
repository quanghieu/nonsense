/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_DictionaryAttackParameters // Command must be defined
#ifndef _DictionaryAttackParameters_H
#define _DictionaryAttackParameters_H

// Input structure definition
typedef struct {
    TPMI_RH_LOCKOUT    lockHandle;
    UINT32             newMaxTries;
    UINT32             newRecoveryTime;
    UINT32             lockoutRecovery;
} DictionaryAttackParameters_In;

// Response code modifiers
#define    RC_DictionaryAttackParameters_lockHandle         (TPM_RC_H + TPM_RC_1)
#define    RC_DictionaryAttackParameters_newMaxTries        (TPM_RC_P + TPM_RC_1)
#define    RC_DictionaryAttackParameters_newRecoveryTime    (TPM_RC_P + TPM_RC_2)
#define    RC_DictionaryAttackParameters_lockoutRecovery    (TPM_RC_P + TPM_RC_3)

// Function prototype
TPM_RC
TPM2_DictionaryAttackParameters(
    DictionaryAttackParameters_In *in
);


#endif  // _DictionaryAttackParameters_H
#endif  // TPM_CC_DictionaryAttackParameters
