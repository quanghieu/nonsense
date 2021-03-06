/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
// In some implementations of the TPM, the hardware can provide a secret
// value to the TPM. This secret value is statistically unique to the
// instance of the TPM. Typical uses of this value are to provide
// personalization to the random number generation and as a shared secret
// between the TPM and the manufacturer.

//** Includes
#include "PlatformData.h"
#include "Platform_fp.h"

const char notReallyUnique[] =
"This is not really a unique value. A real unique value should"
" be generated by the platform.";

//** _plat__GetUnique()
// This function is used to access the platform-specific unique value.
// This function places the unique value in the provided buffer ('b')
// and returns the number of bytes transferred. The function will not
// copy more data than 'bSize'.
// NOTE: If a platform unique value has unequal distribution of uniqueness
// and 'bSize' is smaller than the size of the unique value, the 'bSize'
// portion with the most uniqueness should be returned.
LIB_EXPORT uint32_t
_plat__GetUnique(
    uint32_t             which,         // authorities (0) or details
    uint32_t             bSize,         // size of the buffer
    unsigned char       *b              // output buffer
    )
{
    const char          *from = notReallyUnique;
    uint32_t             retVal = 0;

    if(which == 0) // the authorities value
    {
        for(retVal = 0;
        *from != 0 && retVal < bSize;
            retVal++)
        {
            *b++ = *from++;
        }
    }
    else
    {
#define uSize  sizeof(notReallyUnique)
        b = &b[((bSize < uSize) ? bSize : uSize) - 1];
        for(retVal = 0;
        *from != 0 && retVal < bSize;
            retVal++)
        {
            *b-- = *from++;
        }
    }
    return retVal;
}