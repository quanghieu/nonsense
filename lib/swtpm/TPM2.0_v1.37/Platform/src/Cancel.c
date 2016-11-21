/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//**Introduction
/*
    This module simulates the cancel pins on the TPM.
*/
//** Includes, Typedefs, Structures, and Defines
#include "PlatformData.h"
#include "Platform_fp.h"

//** Functions

//***_plat__IsCanceled()
// Check if the cancel flag is set
// return type: BOOL
//      TRUE(1)      if cancel flag is set
//      FALSE(0)     if cancel flag is not set
LIB_EXPORT int
_plat__IsCanceled(
    void
    )
{
    // return cancel flag
    return s_isCanceled;
}

//***_plat__SetCancel()

// Set cancel flag.
LIB_EXPORT void
_plat__SetCancel(
    void
    )
{
    s_isCanceled = TRUE;
    return;
}

//***_plat__ClearCancel()
// Clear cancel flag
LIB_EXPORT void
_plat__ClearCancel(
    void
    )
{
    s_isCanceled = FALSE;
    return;
}