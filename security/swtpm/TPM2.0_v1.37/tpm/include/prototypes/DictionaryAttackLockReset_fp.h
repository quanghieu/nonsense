/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_DictionaryAttackLockReset // Command must be defined
#ifndef _DictionaryAttackLockReset_H
#define _DictionaryAttackLockReset_H

// Input structure definition
typedef struct {
    TPMI_RH_LOCKOUT    lockHandle;
} DictionaryAttackLockReset_In;

// Response code modifiers
#define    RC_DictionaryAttackLockReset_lockHandle     (TPM_RC_H + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_DictionaryAttackLockReset(
    DictionaryAttackLockReset_In *in
);


#endif  // _DictionaryAttackLockReset_H
#endif  // TPM_CC_DictionaryAttackLockReset
