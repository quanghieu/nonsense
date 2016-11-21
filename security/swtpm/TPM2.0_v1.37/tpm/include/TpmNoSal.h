/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

// The implementation contains macros that are used by MS-specific static analysis
// tools. These macros null those macros so that the implementation will work
// in other environments without having to edit the code.

#ifndef _TPM_NO_SAL_H_
#define _TPM_NO_SAL_H_

#define _Out_writes_( cbData )
#define _Out_writes_bytes_( cbData )
#define _Out_writes_opt_( cbData )
#define _Out_writes_to_(n, c)
#define _In_reads_( cbBytes )
#define _In_reads_opt_( cbAuthData )
#define _In_reads_bytes_(size)
#define _Inout_updates_( nStates )
#define _Inout_updates_bytes_(size)
#define _Field_size_( size )
#define _Field_range_( min, max )
#define _Writable_elements_(c)
#define _Ret_writes_bytes_to_(n, c)

#define _Analysis_assume_(x)
#define _Analysis_noreturn_

#define _Use_decl_annotations_

#endif /* no SAL macros defined */

#ifndef _Interlocked_operand_

#define _Interlocked_operand_

#endif  // _TPM_NO_SAL_H_
