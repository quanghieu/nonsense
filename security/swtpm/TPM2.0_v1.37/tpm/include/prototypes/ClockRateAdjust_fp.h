/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_ClockRateAdjust // Command must be defined
#ifndef _ClockRateAdjust_H
#define _ClockRateAdjust_H

// Input structure definition
typedef struct {
    TPMI_RH_PROVISION    auth;
    TPM_CLOCK_ADJUST     rateAdjust;
} ClockRateAdjust_In;

// Response code modifiers
#define    RC_ClockRateAdjust_auth          (TPM_RC_H + TPM_RC_1)
#define    RC_ClockRateAdjust_rateAdjust    (TPM_RC_P + TPM_RC_1)

// Function prototype
TPM_RC
TPM2_ClockRateAdjust(
    ClockRateAdjust_In *in
);


#endif  // _ClockRateAdjust_H
#endif  // TPM_CC_ClockRateAdjust
