/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Includes and Data Definitions

// This definition allows this module to "see" the values that are private
// to this module but kept in Global.c for ease of state migration.
#define IO_BUFFER_C
#include "Tpm.h"
#include "IoBuffers_fp.h"

// These buffers are set aside to hold command and response values. In this
// implementation, it is not guaranteed that the code will stop accessing
// the s_actionInputBuffer before starting to put values in the
// s_actionOutputBuffer so different buffers are required.
//

//*** MemoryGetActionInputBuffer()
// This function returns the address of the buffer into which the
// command parameters will be unmarshaled in preparation for calling
// the command actions.
BYTE *
MemoryGetActionInputBuffer(
    UINT32           size           // Size, in bytes, required for the input
                                    // unmarshaling
    )
{
    pAssert(size <= sizeof(s_actionInputBuffer));
   // In this implementation, a static buffer is set aside for the command action
   // input buffer.
    memset(s_actionInputBuffer, 0, size);
    return (BYTE *)&s_actionInputBuffer[0];
}

//*** MemoryGetActionOutputBuffer()
// This function returns the address of the buffer into which the command
// action code places its output values.
void *
MemoryGetActionOutputBuffer(
    UINT32           size           // required size of the buffer
    )
{
    pAssert(size < sizeof(s_actionOutputBuffer));
    // In this implementation, a static buffer is set aside for the command action
    // output buffer.
    memset(s_actionOutputBuffer, 0, size);
    return s_actionOutputBuffer;
}

//*** IsLabelProperlyFormatted()
// This function checks that a label is a null-terminated string.
// NOTE: this function is here because there was no better place for it.
// return type: BOOL
//  FALSE   string is not null terminated
//  TRUE    string is null terminated
#ifndef INLINE_FUNCTIONS
BOOL
IsLabelProperlyFormatted(
    TPM2B           *x
    )
{
    return (((x)->size == 0) || ((x)->buffer[(x)->size - 1] == 0));
}
#endif // INLINE_FUNCTIONS
