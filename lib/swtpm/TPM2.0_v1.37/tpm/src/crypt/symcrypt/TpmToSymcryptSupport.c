/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include "Tpm.h"


#if SYM_LIB == SYMCRYPT || HASH_LIB == SYMCRYPT

#include "symcrypt\TpmToSymcryptInclude.h"

#pragma warning( push )
#pragma warning( disable : 4255 )

SYMCRYPT_ENVIRONMENT_GENERIC;

#pragma warning( pop )

#endif

