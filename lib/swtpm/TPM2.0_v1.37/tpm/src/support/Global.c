/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Description
// This file will instance the TPM variables that are not stack allocated. The
// descriptions for these variables is in Global.h.

//** Includes and Defines
#define GLOBAL_C
#include "Tpm.h"

//** Global Data Values

// These values are visible across multiple modules.

BOOL                 g_phEnable;

const UINT16         g_rcIndex[15] = {TPM_RC_1, TPM_RC_2, TPM_RC_3, TPM_RC_4,
                                      TPM_RC_5, TPM_RC_6, TPM_RC_7, TPM_RC_8,
                                      TPM_RC_9, TPM_RC_A, TPM_RC_B, TPM_RC_C,
                                      TPM_RC_D, TPM_RC_E, TPM_RC_F
};

TPM_HANDLE           g_exclusiveAuditSession;

UINT64               g_time;

#ifdef CLOCK_STOPS
CLOCK_NONCE          g_timeEpoch;
#endif
BOOL                 g_timeNewEpochNeeded;

BOOL                 g_pcrReConfig;

TPMI_DH_OBJECT       g_DRTMHandle;

BOOL                 g_DrtmPreStartup;

BOOL                 g_StartupLocality3;

#ifdef USE_DA_USED
BOOL			g_daUsed;
#endif

BOOL                 g_powerWasLost;

BOOL                 g_clearOrderly;

TPM_SU               g_prevOrderlyState;

UPDATE_TYPE          g_updateNV;

BOOL                 g_nvOk;

TPM_RC               g_NvStatus;

TPM2B_AUTH           g_platformUniqueDetails;

STATE_CLEAR_DATA     gc;

STATE_RESET_DATA     gr;

PERSISTENT_DATA      gp;

ORDERLY_DATA         go;

//** Private Values
//*** SessionProcess.c
#ifndef __IGNORE_STATE__        // DO NOT DEFINE THIS VALUE
// These values do not need to be retained between commands.
TPM_HANDLE           s_sessionHandles[MAX_SESSION_NUM];
TPMA_SESSION         s_attributes[MAX_SESSION_NUM];
TPM_HANDLE           s_associatedHandles[MAX_SESSION_NUM];
TPM2B_NONCE          s_nonceCaller[MAX_SESSION_NUM];
TPM2B_AUTH           s_inputAuthValues[MAX_SESSION_NUM];
SESSION             *s_usedSessions[MAX_SESSION_NUM];
UINT32               s_encryptSessionIndex;
UINT32               s_decryptSessionIndex;
UINT32               s_auditSessionIndex;
//UINT32               s_sessionNum;
#endif  // __IGNORE_STATE__

BOOL                 s_DAPendingOnNV;

#ifdef TPM_CC_GetCommandAuditDigest
TPM2B_DIGEST         s_cpHashForCommandAudit;
#endif

//*** DA.c
UINT64               s_selfHealTimer;
UINT64               s_lockoutTimer;

//*** NV.c
UINT64               s_maxCounter;
NV_REF               s_evictNvEnd;
TPM_RC               g_NvStatus;
BYTE                 s_indexOrderlyRam[RAM_INDEX_SPACE];
NV_INDEX             s_cachedNvIndex;
NV_REF               s_cachedNvRef;
BYTE                *s_cachedNvRamRef;

//*** Object.c
OBJECT              s_objects[MAX_LOADED_OBJECTS];

//*** PCR.c
PCR                  s_pcrs[IMPLEMENTATION_PCR];

//*** Session.c
SESSION_SLOT         s_sessions[MAX_LOADED_SESSIONS];
UINT32               s_oldestSavedSession;
int                  s_freeSessionSlots;

//*** MemoryLib.c
// The s_actionOutputBuffer should not be modifiable by the host system until
// the TPM has returned a response code. The s_actionOutputBuffer should not
// be accessible until response parameter encryption, if any, is complete.
// This memory is not used between commands
#ifndef __IGNORE_STATE__        // DO NOT DEFINE THIS VALUE
UINT32   s_actionInputBuffer[1024];          // action input buffer
UINT32   s_actionOutputBuffer[1024];         // action output buffer
#endif

//*** SelfTest.c
ALGORITHM_VECTOR         g_implementedAlgorithms;
ALGORITHM_VECTOR         g_toTest;

//*** g_cryptoSelfTestState
// This structure contains the cryptographic self-test state values.
CRYPTO_SELF_TEST_STATE    g_cryptoSelfTestState;
ALGORITHM_VECTOR          AlgToTest;

//*** TpmFail.c
#ifdef SIMULATION
BOOL                 g_forceFailureMode;
#endif
BOOL                 g_inFailureMode;
UINT32               s_failFunction;
UINT32               s_failLine;
UINT32               s_failCode;

#if 0
#ifdef TPM_ALG_ECC
//*** ECC Curves
ECC_CURVE   c_curves[ECC_CURVE_COUNT];
#endif
#endif // 0

// This is the state used when the library uses a random number generator.
// A special function is installed for the library to call. That function
// picks up the state from this location and uses it for the generation
// of the random number.
RAND_STATE           *s_random;


//*** Manufacture.c
// The values is here rather than in the simulator or platform files in order
// to make it easier to find the TPM state. This is significant when trying to do
// TPM virtualization when the TPM state has to be moved along with virtual machine
// with which it is associated.
BOOL                 g_manufactured = FALSE;

//*** Power.c
// This is here for the same reason that g_manufactured is here. Both of these
// values can be provided by the actual platform-specific code or by hardware
// indications.
BOOL                 g_initialized;

//*** Purpose String Constants
// These string constants are shared across functions to make sure that they 
// are all using consistent sting values.
TPM2B_STRING(PRIMARY_OBJECT_CREATION, "Primary Object Creation");
TPM2B_STRING(CFB_KEY, "CFB");
TPM2B_STRING(CONTEXT_KEY, "CONTEXT");
TPM2B_STRING(INTEGRITY_KEY, "INTEGRITY");
TPM2B_STRING(SECRET_KEY, "SECRET");
TPM2B_STRING(SESSION_KEY, "ATH");
TPM2B_STRING(STORAGE_KEY, "STORAGE");
TPM2B_STRING(XOR_KEY, "XOR");
TPM2B_STRING(COMMIT_STRING, "ECDAA Commit");
TPM2B_STRING(DUPLICATE_STRING, "DUPLICATE");
TPM2B_STRING(IDENTITY_STRING, "IDENTITY");
TPM2B_STRING(OBFUSCATE_STRING, "OBFUSCATE");
#ifdef SELF_TEST
TPM2B_STRING(OAEP_TEST_STRING, "OAEP Test Value");
#endif // SELF_TEST








