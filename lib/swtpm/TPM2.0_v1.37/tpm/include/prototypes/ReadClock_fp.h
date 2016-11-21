/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)

    Created by TpmStructures Version 2.7 Sept 13, 2016
    This file created on Sep 22, 2016, 05:27:02PM 

*/

#ifdef TPM_CC_ReadClock // Command must be defined
#ifndef _ReadClock_H
#define _ReadClock_H

// Output structure definition
typedef struct {
    TPMS_TIME_INFO    currentTime;
} ReadClock_Out;


// Function prototype
TPM_RC
TPM2_ReadClock(
    ReadClock_Out *out
);


#endif  // _ReadClock_H
#endif  // TPM_CC_ReadClock
