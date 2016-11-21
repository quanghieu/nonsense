/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_FieldUpgradeData // Command must be defined
#ifndef _FieldUpgradeData_H
#define _FieldUpgradeData_H

// Input structure definition
typedef struct {
    TPM2B_MAX_BUFFER    fuData;
} FieldUpgradeData_In;

// Output structure definition
typedef struct {
    TPMT_HA             nextDigest;
    TPMT_HA             firstDigest;
} FieldUpgradeData_Out;

// Response code modifiers
#define    RC_FieldUpgradeData_fuData    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_FieldUpgradeData(
    FieldUpgradeData_In *in,
    FieldUpgradeData_Out *out
);


#endif  // _FieldUpgradeData_H
#endif  // TPM_CC_FieldUpgradeData
