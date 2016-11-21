/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


//** Description
// This file contains the entry point for the simulator.

//** Includes, Defines, Data Definitions, and Function Prototypes

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include <strsafe.h>
#include "string.h"

extern "C" {
    #include "TpmTcpProtocol.h"
}

#define PURPOSE \
"TPM Reference Simulator.\nCopyright Microsoft 2010, 2011.\n"

#define DEFAULT_TPM_PORT 2321

void* MainPointer;

extern "C" {
    int TPM_Manufacture();
    int _plat__NVEnable(void* platParameters);
    void _plat__NVDisable();
    int StartTcpServer(int PortNumber);
}

//** Functions

//*** Usage()
// This function prints the proper calling sequence for the simulator.
void Usage(char * pszProgramName)
{
    fprintf_s(stderr, "%s", PURPOSE);
    fprintf_s(stderr, "Usage:\n");
    fprintf_s(stderr, "%s         - Starts the TPM server listening on port %d\n", 
              pszProgramName, DEFAULT_TPM_PORT);
    fprintf_s(stderr, 
              "%s PortNum - Starts the TPM server listening on port PortNum\n", 
              pszProgramName);
    fprintf_s(stderr, "%s ?       - This message\n", pszProgramName);
    exit(1);
}

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
    public:

        TEST_METHOD(RunSimulator)
        {
            _plat__NVEnable(NULL);
            if(TPM_Manufacture() != 0)
            {
                exit(RPC_S_INTERNAL_ERROR);
            }
            // Disable NV memory
            _plat__NVDisable();

            StartTcpServer(DEFAULT_TPM_PORT);
		}

	};
}
