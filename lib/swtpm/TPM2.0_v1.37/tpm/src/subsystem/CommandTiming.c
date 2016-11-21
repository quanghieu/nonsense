/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
// This file contains the functions relating to the TPM's time functions including
// the interface to the implementation-specific time functions.
//
//** Includes
#include "Tpm.h"
#include <time.h>
#include "PlatformData.h"


typedef struct {
    clock_t         passTime;
    clock_t         failTime;
    int             passCount;
    int             failCount;
    TPM_CC          commandCode;
} commandTime_t;

clock_t             commandStartTime;
commandTime_t       commandTimes[LIBRARY_COMMAND_ARRAY_SIZE] = {0};

void
CommandStartTime(
    void
    )
{
//    clock_t          ready = clock();
    // Attempt to hit as close to the start of a CLOCK_TICK as possible.
//    while((commandStartTime = clock()) == ready);
    commandStartTime = clock();
}

void
CommandEndTime(
    TPM_CC          commandCode,
    TPM_RC          response
    )
{
    clock_t         elapsedTime;
    COMMAND_INDEX   index;
    elapsedTime = clock() - commandStartTime;
    index = CommandCodeToCommandIndex(commandCode);
    if(UNIMPLEMENTED_COMMAND_INDEX == index)
        return;
    commandTimes[index].commandCode = commandCode;
    if(response == TPM_RC_SUCCESS)
    {
        commandTimes[index].passTime += elapsedTime;
        commandTimes[index].passCount++;
    }
    else
    {
        commandTimes[index].failTime += elapsedTime;
        commandTimes[index].failCount++;
    }

}

void
CommandReportTime(
    void
    )
{
    FILE    *fp = fopen("CommandTimings.txt", "w");
    if(fp != NULL)
    {
        int         i;
        float       clocksPerMs = (float)CLOCKS_PER_SEC / 1000.0;

        for(i = 0; i < LIBRARY_COMMAND_ARRAY_SIZE; i++)
        {
            float       failAverage = 0.0;
            float       passAverage = 0.0;
            if((commandTimes[i].commandCode == 0)
               || ((commandTimes[i].passCount + commandTimes[i].failCount) == 0))
                continue;
            if(commandTimes[i].passCount > 0)
                passAverage = (float)commandTimes[i].passTime
                / commandTimes[i].passCount / clocksPerMs;
            if(commandTimes[i].failCount > 0)
                failAverage = (float)commandTimes[i].failTime
                / commandTimes[i].failCount / clocksPerMs;


            fprintf(fp, "0x%04x, %3d, %9.3fms, %3d, %9.3fms\n",
                    commandTimes[i].commandCode,
                    commandTimes[i].passCount, passAverage,
                    commandTimes[i].failCount, failAverage);
        }
        fclose(fp);
    }
    else
        printf("Can't open CommandTimings.txt\n");
}