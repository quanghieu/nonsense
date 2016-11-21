/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

// Root header file for building any TPM.lib code

#ifndef     _TPM_H_
#define     _TPM_H_

#include "Implementation.h"
#include "LibSupport.h"         // Types from the library. These need to come before
                                // Global.h because some of the structures in
                                // that file depend on the structures used by the
                                // crypto libraries.
#include "GpMacros.h"           // Define additional macros
#include "Global.h"             // Define other TPM types
#include "InternalRoutines.h"   // Function prototypes

#endif // _TPM_H_
