/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#include "Tpm.h"
#include "SequenceComplete_fp.h"

#ifdef TPM_CC_SequenceComplete  // Conditional expansion of this file

/*(See part 3 specification)
// Complete a sequence and flush the object.
*/
// return type: TPM_RC
//      TPM_RC_TYPE             'sequenceHandle' does not reference a hash or HMAC
//                              sequence object
TPM_RC
TPM2_SequenceComplete(
    SequenceComplete_In     *in,            // IN: input parameter list
    SequenceComplete_Out    *out            // OUT: output parameter list
    )
{
    OBJECT                      *object;

// Input validation

    // Get hash object pointer
    object = HandleToObject(in->sequenceHandle);

    // input handle must be a hash or HMAC sequence object.
    if(object->attributes.hashSeq == CLEAR
       && object->attributes.hmacSeq == CLEAR)
        return TPM_RCS_MODE + RC_SequenceComplete_sequenceHandle;

// Command Output

    if(object->attributes.hashSeq == SET)           // sequence object for hash
    {
        // Update last piece of data
        HASH_OBJECT     *hashObject = (HASH_OBJECT *)object;

       // Get the hash algorithm before the algorithm is lost in CryptHashEnd
        TPM_ALG_ID       hashAlg = hashObject->state.hashState[0].hashAlg;

        CryptDigestUpdate2B(&hashObject->state.hashState[0], &in->buffer.b);

        // Complete hash
        out->result.t.size
            = CryptHashGetDigestSize(
                CryptHashGetContextAlg(&hashObject->state.hashState[0]));

        CryptHashEnd2B(&hashObject->state.hashState[0], &out->result.b);

        // Check if the first block of the sequence has been received
        if(hashObject->attributes.firstBlock == CLEAR)
        {
            // If not, then this is the first block so see if it is 'safe'
            // to sign.
            if(TicketIsSafe(&in->buffer.b))
                hashObject->attributes.ticketSafe = SET;
        }

        // Output ticket
        out->validation.tag = TPM_ST_HASHCHECK;
        out->validation.hierarchy = in->hierarchy;

        if(in->hierarchy == TPM_RH_NULL)
        {
            // Ticket is not required
            out->validation.digest.t.size = 0;
        }
        else if(object->attributes.ticketSafe == CLEAR)
        {
            // Ticket is not safe to generate
            out->validation.hierarchy = TPM_RH_NULL;
            out->validation.digest.t.size = 0;
        }
        else
        {
            // Compute ticket
            TicketComputeHashCheck(out->validation.hierarchy, hashAlg,
                                   &out->result, &out->validation);
        }
    }
    else
    {
        HASH_OBJECT     *hashObject = (HASH_OBJECT *)object;

        //   Update last piece of data
        CryptDigestUpdate2B(&hashObject->state.hmacState.hashState, &in->buffer.b);
        // Complete hash/HMAC
        out->result.t.size =
            CryptHashGetDigestSize(
                CryptHashGetContextAlg(&hashObject->state.hmacState.hashState));
        CryptHmacEnd2B(&(hashObject->state.hmacState), &out->result.b);

        // No ticket is generated for HMAC sequence
        out->validation.tag = TPM_ST_HASHCHECK;
        out->validation.hierarchy = TPM_RH_NULL;
        out->validation.digest.t.size = 0;
    }

// Internal Data Update

    // mark sequence object as evict so it will be flushed on the way out
    object->attributes.evict = SET;

    return TPM_RC_SUCCESS;
}

#endif // CC_SequenceComplete