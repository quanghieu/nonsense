/*(Copyright)
 *      Microsoft Copyright 2009 - 2015
 *      All rights reserved.
*/
/*(Auto)
    Automatically Generated by TpmPrototypes version 2.2 February 10, 2016 
    Date: Sep 22, 2016 Time: 05:27:05 PM  
*/

#ifndef    _COMMANDTIMING_FP_H_
#define    _COMMANDTIMING_FP_H_

void
CommandStartTime(
    void
    );

void
CommandEndTime(
    TPM_CC          commandCode,
    TPM_RC          response
    );

void
CommandReportTime(
    void
    );


#endif  // _COMMANDTIMING_FP_H_
