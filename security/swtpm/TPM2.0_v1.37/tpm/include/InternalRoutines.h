/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#ifndef     INTERNAL_ROUTINES_H
#define     INTERNAL_ROUTINES_H

#if !defined _LIB_SUPPORT_H_ && !defined _TPM_H_
#error "Should not be called"
#endif

// DRTM functions
#include "_TPM_Hash_Start_fp.h"
#include "_TPM_Hash_Data_fp.h"
#include "_TPM_Hash_End_fp.h"

// Internal subsystem functions
#include "Object_fp.h"
#include "Context_spt_fp.h"
#include "Object_spt_fp.h"
#include "Entity_fp.h"
#include "Session_fp.h"
#include "Hierarchy_fp.h"
#include "NvReserved_fp.h"
#include "NvDynamic_fp.h"
#include "NV_spt_fp.h"
#include "PCR_fp.h"
#include "DA_fp.h"
#include "TpmFail_fp.h"
#include "SessionProcess_fp.h"

// Internal support functions
#include "CommandCodeAttributes_fp.h"
#include "Marshal_fp.h"
#include "Time_fp.h"
#include "Locality_fp.h"
#include "PP_fp.h"
#include "CommandAudit_fp.h"
#include "Manufacture_fp.h"
#include "Handle_fp.h"
#include "Power_fp.h"
#include "Response_fp.h"

// Miscellaneous
#include "Bits_fp.h"
#include "AlgorithmCap_fp.h"
#include "PropertyCap_fp.h"
#include "IoBuffers_fp.h"
#include "Memory_fp.h"
#include "ResponseCodeProcessing_fp.h"

// Internal crypto functions
#include "BnConvert_fp.h"
#include "BnMath_fp.h"
#include "BnMemory_fp.h"
#include "Ticket_fp.h"
#include "CryptUtil_fp.h"
#include "CryptHash_fp.h"
#include "CryptSym_fp.h"
#include "CryptDes_fp.h"
#include "CryptPrime_fp.h"
#include "CryptRand_fp.h"
#include "CryptSelfTest_fp.h"
#include "MathOnByteBuffers_fp.h"
#include "CryptSym_fp.h"
#include "AlgorithmTests_fp.h"

#ifdef TPM_ALG_RSA
#include "CryptRsa_fp.h"
#include "CryptPrimeSieve_fp.h"
#endif

#ifdef TPM_ALG_ECC
#include "CryptEccMain_fp.h"
#include "CryptEccSignature_fp.h"
#include "CryptEccKeyExchange_fp.h"
#endif

// Support library
#include "SupportLibraryFunctionPrototypes_fp.h"

// Linkage to platform functions
#include "Platform_fp.h"

#endif
