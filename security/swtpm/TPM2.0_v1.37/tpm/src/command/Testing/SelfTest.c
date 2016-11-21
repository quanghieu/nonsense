/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include "Tpm.h"
#include "SelfTest_fp.h"

#ifdef TPM_CC_SelfTest  // Conditional expansion of this file

/*(See part 3 specification)
// perform a test of TPM capabilities
*/
// return type: TPM_RC
//      TPM_RC_CANCELED             the command was canceled (some incremental
//                                  process may have been made)
//      TPM_RC_TESTING              self test in process
TPM_RC
TPM2_SelfTest(
    SelfTest_In     *in             // IN: input parameter list
    )
{
// Command Output

    // Call self test function in crypt module
    return CryptSelfTest(in->fullTest);
}

#endif // CC_SelfTest