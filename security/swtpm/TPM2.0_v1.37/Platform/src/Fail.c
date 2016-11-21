/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//**Introduction
// This module provides the platform specific fail processing. 
// The _plat__Fail() function is called when the platform does not allow setjmp/longjmp. The
// expectation is that this code will exit to the system with a value that will
// indicate that the system should call the TPM again. On this call, the TPM will
// be in Failure mode and it will return the failure mode indication.

//** Includes, Typedefs, Structures, and Defines
#include <stdlib.h>
#include "PlatformData.h"
#include "Platform_fp.h"

//** Functions

//***_plat__Fail()
LIB_EXPORT NORETURN void
_plat__Fail(
    void
    )
{
// Insert the appropriate system call here.
    exit(1);
}