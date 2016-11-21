/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Description
// This file contains the entry point for the simulator.

//** Includes, Defines, Data Definitions, and Function Prototypes
#include "TpmBuildSwitches.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

#include <windows.h>
#include <winsock.h>

#include "TpmTcpProtocol.h"
#include "Manufacture_fp.h"
#include "Platform_fp.h"
#include "Simulator_fp.h"

#define PURPOSE \
"TPM Reference Simulator.\nCopyright Microsoft 2010, 2011.\n"

#define DEFAULT_TPM_PORT 2321

void* MainPointer;

//** Functions

//*** Usage()
// This function prints the proper calling sequence for the simulator.
static void
Usage(
    char                *pszProgramName
    )
{
    fprintf(stderr, "%s", PURPOSE);
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "%s         - Starts the TPM server listening on port %d\n",
              pszProgramName, DEFAULT_TPM_PORT);
    fprintf(stderr,
              "%s PortNum - Starts the TPM server listening on port PortNum\n",
              pszProgramName);
    fprintf(stderr, "%s ?       - This message\n", pszProgramName);
    exit(1);
}

//*** main()
// This is the main entry point for the simulator.
/* main:  register the interface, start listening for clients */
int
main(
    int              argc,
    char            *argv[]
    )
{
    int portNum = DEFAULT_TPM_PORT;
    if(argc > 2)
    {
        Usage(argv[0]);
    }

    if(argc == 2)
    {
        if(strcmp(argv[1], "?") == 0)
        {
            Usage(argv[0]);
        }
        portNum = atoi(argv[1]);
        if(portNum <= 0 || portNum > 65535)
        {
            Usage(argv[0]);
        }
    }
    _plat__NVEnable(NULL);

    if(TPM_Manufacture(1) != 0)
    {
        exit(1);
    }
    // Coverage test - repeated manufacturing attempt
    if(TPM_Manufacture(0) != 1)
    {
        exit(2);
    }
    // Coverage test - re-manufacturing
    TPM_TearDown();
    if(TPM_Manufacture(1) != 0)
    {
        exit(3);
    }
    // Disable NV memory
    _plat__NVDisable();

    StartTcpServer(portNum);
    return EXIT_SUCCESS;
}