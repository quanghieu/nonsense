/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Introduction
// This file contains the functions used for managing and accessing the
// hierarchy-related values.

//** Includes

#include "Tpm.h"

//** Functions

//*** HierarchyPreInstall()
// This function performs the initialization functions for the hierarchy
// when the TPM is simulated. This function should not be called if the
// TPM is not in a manufacturing mode at the manufacturer, or in a simulated
// environment.
void
HierarchyPreInstall_Init(
    void
    )
{
    // Allow lockout clear command
    gp.disableClear = FALSE;

    // Initialize Primary Seeds
    gp.EPSeed.t.size = PRIMARY_SEED_SIZE;
    CryptRandomGenerate(PRIMARY_SEED_SIZE, gp.EPSeed.t.buffer);
    gp.SPSeed.t.size = PRIMARY_SEED_SIZE;
    CryptRandomGenerate(PRIMARY_SEED_SIZE, gp.SPSeed.t.buffer);
    gp.PPSeed.t.size = PRIMARY_SEED_SIZE;
#ifdef USE_PLATFORM_EPS
    _plat__GetEPS(PRIMARY_SEED_SIZE, gp.EPSeed.t.buffer);
#else
    CryptRandomGenerate(PRIMARY_SEED_SIZE, gp.PPSeed.t.buffer);
#endif

    // Initialize owner, endorsement and lockout auth
    gp.ownerAuth.t.size = 0;
    gp.endorsementAuth.t.size = 0;
    gp.lockoutAuth.t.size = 0;

    // Initialize owner, endorsement, and lockout policy
    gp.ownerAlg = TPM_ALG_NULL;
    gp.ownerPolicy.t.size = 0;
    gp.endorsementAlg = TPM_ALG_NULL;
    gp.endorsementPolicy.t.size = 0;
    gp.lockoutAlg = TPM_ALG_NULL;
    gp.lockoutPolicy.t.size = 0;

    // Initialize ehProof, shProof and phProof
    gp.phProof.t.size = PROOF_SIZE;
    gp.shProof.t.size = PROOF_SIZE;
    gp.ehProof.t.size = PROOF_SIZE;
    CryptRandomGenerate(gp.phProof.t.size, gp.phProof.t.buffer);
    CryptRandomGenerate(gp.shProof.t.size, gp.shProof.t.buffer);
    CryptRandomGenerate(gp.ehProof.t.size, gp.ehProof.t.buffer);

    // Write hierarchy data to NV
    NV_SYNC_PERSISTENT(disableClear);
    NV_SYNC_PERSISTENT(EPSeed);
    NV_SYNC_PERSISTENT(SPSeed);
    NV_SYNC_PERSISTENT(PPSeed);
    NV_SYNC_PERSISTENT(ownerAuth);
    NV_SYNC_PERSISTENT(endorsementAuth);
    NV_SYNC_PERSISTENT(lockoutAuth);
    NV_SYNC_PERSISTENT(ownerAlg);
    NV_SYNC_PERSISTENT(ownerPolicy);
    NV_SYNC_PERSISTENT(endorsementAlg);
    NV_SYNC_PERSISTENT(endorsementPolicy);
    NV_SYNC_PERSISTENT(lockoutAlg);
    NV_SYNC_PERSISTENT(lockoutPolicy);
    NV_SYNC_PERSISTENT(phProof);
    NV_SYNC_PERSISTENT(shProof);
    NV_SYNC_PERSISTENT(ehProof);

    return;
}

//*** HierarchyStartup()
// This function is called at TPM2_Startup() to initialize the hierarchy
// related values.
void
HierarchyStartup(
    STARTUP_TYPE     type           // IN: start up type
    )
{
    // phEnable is SET on any startup
    g_phEnable = TRUE;

    // Reset platformAuth, platformPolicy; enable SH and EH at TPM_RESET and
    // TPM_RESTART
    if(type != SU_RESUME)
    {
        gc.platformAuth.t.size = 0;
        gc.platformPolicy.t.size = 0;

        // enable the storage and endorsement hierarchies and the platformNV
        gc.shEnable = gc.ehEnable = gc.phEnableNV = TRUE;
    }

    // nullProof and nullSeed are updated at every TPM_RESET
    if((type != SU_RESTART) && (type != SU_RESUME))
    {
        gr.nullProof.t.size = PROOF_SIZE;
        CryptRandomGenerate(gr.nullProof.t.size,
                            gr.nullProof.t.buffer);
        gr.nullSeed.t.size = PRIMARY_SEED_SIZE;
        CryptRandomGenerate(PRIMARY_SEED_SIZE, gr.nullSeed.t.buffer);
    }

    return;
}

//*** HierarchyGetProof()
// This function finds the proof value associated with a hierarchy.It returns a
// pointer to the proof value.
TPM2B_AUTH *
HierarchyGetProof(
    TPMI_RH_HIERARCHY    hierarchy      // IN: hierarchy constant
    )
{
    TPM2B_AUTH          *auth = NULL;

    switch(hierarchy)
    {
        case TPM_RH_PLATFORM:
            // phProof for TPM_RH_PLATFORM
            auth = &gp.phProof;
            break;
        case TPM_RH_ENDORSEMENT:
            // ehProof for TPM_RH_ENDORSEMENT
            auth = &gp.ehProof;
            break;
        case TPM_RH_OWNER:
            // shProof for TPM_RH_OWNER
            auth = &gp.shProof;
            break;
        case TPM_RH_NULL:
            // nullProof for TPM_RH_NULL
            auth = &gr.nullProof;
            break;
        default:
            FAIL(FATAL_ERROR_INTERNAL);
            break;
    }
    return auth;
}

//*** HierarchyGetPrimarySeed()
// This function returns the primary seed of a hierarchy.
TPM2B_SEED *
HierarchyGetPrimarySeed(
    TPMI_RH_HIERARCHY    hierarchy      // IN: hierarchy
    )
{
    TPM2B_SEED          *seed = NULL;
    switch(hierarchy)
    {
        case TPM_RH_PLATFORM:
            seed = &gp.PPSeed;
            break;
        case TPM_RH_OWNER:
            seed = &gp.SPSeed;
            break;
        case TPM_RH_ENDORSEMENT:
            seed = &gp.EPSeed;
            break;
        case TPM_RH_NULL:
            return &gr.nullSeed;
        default:
            FAIL(FATAL_ERROR_INTERNAL);
            break;
    }
    return seed;
}

//*** HierarchyIsEnabled()
// This function checks to see if a hierarchy is enabled.
// NOTE: The TPM_RH_NULL hierarchy is always enabled.
// return type: BOOL
//  TRUE           hierarchy is enabled
//  FALSE          hierarchy is disabled
BOOL
HierarchyIsEnabled(
    TPMI_RH_HIERARCHY    hierarchy      // IN: hierarchy
    )
{
    BOOL            enabled = FALSE;

    switch(hierarchy)
    {
        case TPM_RH_PLATFORM:
            enabled = g_phEnable;
            break;
        case TPM_RH_OWNER:
            enabled = gc.shEnable;
            break;
        case TPM_RH_ENDORSEMENT:
            enabled = gc.ehEnable;
            break;
        case TPM_RH_NULL:
            enabled = TRUE;
            break;
        default:
            FAIL(FATAL_ERROR_INTERNAL);
            break;
    }
    return enabled;
}