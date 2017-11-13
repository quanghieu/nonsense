/******************************************************************************
 * This file is provided under a dual BSD/GPLv2 license.  When using or
 * redistributing this file, you may do so under either license.
 *
 * GPL LICENSE SUMMARY
 *
 * Copyright(c) 2016 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * The full GNU General Public License is included in this distribution
 * in the file called LICENSE.GPL.
 *
 * Contact Information:
 *	Intel Corporation.
 *	linux-mei@linux.intel.com
 *	http://www.intel.com
 *
 * BSD LICENSE
 *
 * Copyright(c) 2016 Intel Corporation. All rights reserved.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name Intel Corporation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/
#include <linux/delay.h>

#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <linux/rpmb.h>
#include <linux/slab.h>

#include "linux/rpmb.h"
#include "rpmb.h"
#include "helper.h"
#include "aes.h"
#include <linux/rpmb.h>

#include "Tpm.h"
#include "CryptHash_fp.h"

#define SEED_SIZE       32
#define RPMB_KEY_SIZE   32
#define RPMB_MAC_SIZE   32
#define RPMB_NONCE_SIZE 16

bool verbose;
#define rpmb_dbg(fmt, ARGS...) do {         \
	if (verbose)                             \
		pr_debug("rpmb: " fmt, ##ARGS);       \
} while (0)

#define rpmb_msg(fmt, ARGS...) \
	pr_info("rpmb: " fmt, ##ARGS)

#define rpmb_err(fmt, ARGS...) \
	pr_err("rpmb: error: " fmt, ##ARGS)

#define fprintf(out, fmt, ARGS...) \
    pr_info(fmt, ##ARGS)

#define calloc(cnt, size)   malloc(cnt * size)

// endian
#define be16toh __be16_to_cpu
#define be32toh __be32_to_cpu
#define htobe16 __cpu_to_be16
#define htobe32 __cpu_to_be32

static TPM2B_SEED CPSeed;   // Cloud primary seed
static char CLOUD_SEED[] = {
    0x97, 0xC2, 0x3A, 0x21, 0x3E, 0x9A, 0xBD, 0x9E,
    0x95, 0xFC, 0x4E, 0xAD, 0x7A, 0x03, 0xE1, 0xCC,
    0x17, 0x5D, 0x50, 0xA8, 0xF2, 0x7E, 0xE5, 0x51,
    0x77, 0x17, 0x75, 0xE0, 0x7D, 0x3E, 0x88, 0x6E,
};
static TPM2B_NAME name;
static char NAME_BUFFER[] = {
    0x00, 0x0B, 0xA2, 0x14, 0x6E, 0xD1, 0x58, 0xC9,
    0xDD, 0xEF, 0x76, 0x98, 0xEE, 0x38, 0x3E, 0xD0,
    0x68, 0x87, 0xDB, 0x31, 0x79, 0xE4, 0x8B, 0x9A,
    0xEF, 0xF5, 0xFF, 0x37, 0xA8, 0x77, 0x92, 0x6D,
    0xF9, 0x44,
};
// symKey must be  57 F0 3A 27 B0 50 87 D2 08 81 59 BC C1 84 19 9C
static TPM2B_SYM_KEY    symKey;
// hmacKey must be AE 00 95 98 67 94 18 49 28 85 E1 7C 11 6F 86 D6
//                 61 2F A6 D4 80 00 49 D3 99 CD 4C A7 31 E2 37 10
static TPM2B_DIGEST     hmacKey;

static uint32_t rpmb_write_counter;
static uint32_t cloud_write_counter;
struct rpmb_seq_ioc {
        struct rpmb_ioc_seq_cmd h;
        struct rpmb_ioc_cmd cmd[3];
}; 
static const char *rpmb_op_str(uint16_t op)
{
#define RPMB_OP(_op) case RPMB_##_op: return #_op

    switch (op) {
        RPMB_OP(PROGRAM_KEY);
        RPMB_OP(GET_WRITE_COUNTER);
        RPMB_OP(WRITE_DATA);
        RPMB_OP(READ_DATA);
        RPMB_OP(RESULT_READ);
        break;
        default:
        return "unknown";
    }
#undef RPMB_OP
}

static const char *rpmb_result_str(enum rpmb_op_result result)
{
#define str(x) #x
#define RPMB_ERR(_res) case RPMB_ERR_##_res:         \
    { if (result & RPMB_ERR_COUNTER_EXPIRED)     \
        return "COUNTER_EXPIRE:" str(_res);  \
    else                                         \
        return #_res;                        \
    }

    switch (result & 0x000F) {
        RPMB_ERR(OK);
        RPMB_ERR(GENERAL);
        RPMB_ERR(AUTH);
        RPMB_ERR(COUNTER);
        RPMB_ERR(ADDRESS);
        RPMB_ERR(WRITE);
        RPMB_ERR(READ);
        RPMB_ERR(NO_KEY);
        break;
        default:
        return "unknown";
    }
#undef RPMB_ERR
#undef str
};

static inline void __dump_buffer(const char *buf)
{
	pr_info("%s\n", buf);
}

static void
dump_hex_buffer(const char *title, const void *buf, size_t len)
{
	const unsigned char *_buf = (const unsigned char *)buf;
	const size_t pbufsz = 16 * 3;
	char pbuf[pbufsz];
	int j = 0;

	if (title)
		pr_info("%s\n", title);
	while (len-- > 0) {
		snprintf(&pbuf[j], pbufsz - j, "%02X ", *_buf++);
		j += 3;
		if (j == 16 * 3) {
			__dump_buffer(pbuf);
			j = 0;
		}
	}
	if (j)
		__dump_buffer(pbuf);
}

void dbg_dump_frame(const char *title, const struct rpmb_frame *f)
{
	uint16_t result, req_resp;

//	if (!verbose)
//		return;

	if (!f)
		return;

	if (title)
		fprintf(stderr, "%s\n", title);

	result = be16toh(f->result);
	req_resp = be16toh(f->req_resp);
	if (req_resp & 0xf00)
		req_resp = RPMB_RESP2REQ(req_resp);

	fprintf(stderr, "ptr: %p\n", f);
	dump_hex_buffer("key_mac: ", f->key_mac, 32);
	dump_hex_buffer("data: ", f->data, 256);
	dump_hex_buffer("nonce: ", f->nonce, 16);
	fprintf(stderr, "write_counter: %u\n", be32toh(f->write_counter));
	fprintf(stderr, "address:  %0X\n", be16toh(f->addr));
	fprintf(stderr, "block_count: %u\n", be16toh(f->block_count));
	fprintf(stderr, "result %s:%d\n", rpmb_result_str(result), result);
	fprintf(stderr, "req_resp %s\n", rpmb_op_str(req_resp));
}

void init_rpmb(bool new_verbose)
{
    TPMI_ALG_HASH   hashAlg = ALG_SHA256_VALUE;

    verbose = new_verbose;

    // Set Cloud Seed structure
    CPSeed.t.size = SEED_SIZE;
    memcpy(CPSeed.t.buffer, CLOUD_SEED, SEED_SIZE);
    dump_hex_buffer("CPSeed", CPSeed.t.buffer, CPSeed.t.size);
    
    // Set name of symmetric key
    name.t.size = sizeof(NAME_BUFFER);
    memcpy(name.t.name, NAME_BUFFER, sizeof(NAME_BUFFER));
    dump_hex_buffer("name", name.t.name, name.t.size);

    // KDFa to generate symmetric key and IV value
    symKey.t.size = 16;  // the symmetric key size in bits, 128 bits
    CryptKDFa(ALG_SHA256_VALUE, &CPSeed.b, STORAGE_KEY, &name.b, NULL,
            symKey.t.size * 8, symKey.t.buffer, NULL, FALSE);
    dump_hex_buffer("symKey", symKey.t.buffer, symKey.t.size);

    // Determine the HMAC key bits
    hmacKey.t.size = CryptHashGetDigestSize(hashAlg);
    // KDFa to generate HMAC key
    CryptKDFa(hashAlg, &CPSeed.b, INTEGRITY_KEY, NULL, NULL,
            hmacKey.t.size * 8, hmacKey.t.buffer, NULL, FALSE);
    dump_hex_buffer("hmacKey", hmacKey.t.buffer, hmacKey.t.size);

    // initialize rpmb write_counter as 0
    rpmb_write_counter = 0;
    cloud_write_counter = 0;
}


//** Internally used functions

static struct rpmb_frame *rpmb_alloc_frames(unsigned int cnt)
{
    struct rpmb_frame *ptr = calloc(cnt, sizeof(struct rpmb_frame));
    memset(ptr, 0, cnt * sizeof(struct rpmb_frame));
	return ptr;
}

static int rpmb_calc_hmac_sha256(struct rpmb_frame *frames, size_t blocks_cnt,
				 const unsigned char key[],
				 unsigned int key_size,
				 unsigned char mac[],
				 unsigned int mac_size)
{
	HMAC_CTX ctx;
	int ret;
	unsigned int i;

	 /* SSL returns 1 on success 0 on failure */

	HMAC_CTX_init(&ctx);
	ret = HMAC_Init_ex(&ctx, key, key_size, EVP_sha256(), NULL);
	if (ret == 0)
		goto out;
	for (i = 0; i < blocks_cnt; i++)
		HMAC_Update(&ctx, frames[i].data, hmac_data_len);

	ret = HMAC_Final(&ctx, mac, &mac_size);
	if (ret == 0)
		goto out;
	if (mac_size != RPMB_MAC_SIZE)
		ret = 0;

	ret = 1;
out:
	HMAC_CTX_cleanup(&ctx);
	return ret == 1 ? 0 : -1;
}

static int rpmb_check_req_resp(uint16_t req, struct rpmb_frame *frame_out)
{
	if (RPMB_REQ2RESP(req) != be16toh(frame_out->req_resp)) {
		rpmb_err("RPMB response mismatch %04X != %04X\n",
			 RPMB_REQ2RESP(req), be16toh(frame_out->req_resp));
		return -1;
	}
	return 0;
}

static int rpmb_check_mac(const unsigned char *key,
			  struct rpmb_frame *frames_out,
			  unsigned int cnt_out)
{
	unsigned char mac[RPMB_MAC_SIZE];

	if (cnt_out == 0) {
		rpmb_err("RPMB 0 output frames\n");
		return -1;
	}

	rpmb_calc_hmac_sha256(frames_out, cnt_out,
			      key, RPMB_KEY_SIZE,
			      mac, RPMB_MAC_SIZE);

	if (memcmp(mac, frames_out[cnt_out - 1].key_mac, RPMB_MAC_SIZE)) {
		rpmb_err("RPMB hmac mismatch\n");
		dump_hex_buffer("Result MAC: ",
				frames_out[cnt_out - 1].key_mac, RPMB_MAC_SIZE);
		dump_hex_buffer("Expected MAC: ", mac, RPMB_MAC_SIZE);
		return -1;
	}

	return 0;
}

static int rpmb_cmd_copy(struct rpmb_cmd *cmd,
			struct rpmb_ioc_cmd *ucmd)
{
	size_t sz;
	struct rpmb_frame *frames;
	u64 frames_ptr;

	memcpy(cmd,ucmd,sizeof(u32) + sizeof(u32));
	pr_info("Number of frame: %u\n",cmd->nframes);
	if (cmd->nframes > 255)
		return -EOVERFLOW;

	if (!cmd->nframes)
		return -EINVAL;

	/* some archs have issues with 64bit get_user */
//	memcpy(&frames_ptr, ucmd->frames_ptr, sizeof(frames_ptr));
//	pr_info("Copy 1\n");
//	dbg_dump_frame("User frame: ",&(ucmd->frames_ptr));
	sz = cmd->nframes * sizeof(struct rpmb_frame);
	frames = kmalloc(sz, GFP_KERNEL);
	memcpy(frames, &ucmd->frames_ptr, sz);
	pr_info("Copy 2\n");
	cmd->frames = frames;
	dbg_dump_frame("Input frame: ",frames);
	return 0;
}

static long rpmb_seq_cmd(struct rpmb_dev *rdev,
			       struct rpmb_seq_ioc *ptr)
{
	__u64 ncmds;
	struct rpmb_cmd *cmds;
	struct rpmb_ioc_cmd *ucmds;

	int i;
	int ret;

	ncmds = ptr->h.num_of_cmds;
	if (ncmds > 3) {
		dev_err(&rdev->dev, "supporting up to 3 packets (%llu)\n",
			ncmds);
		return -EINVAL;
	}

	cmds = kcalloc(ncmds, sizeof(*cmds), GFP_KERNEL);
	if (!cmds)
		return -ENOMEM;

	ucmds = (struct rpmb_ioc_cmd *)(ptr->cmd);
	
	for (i = 0; i < ncmds; i++) {
		rpmb_cmd_copy(&cmds[i], &ucmds[i]);
//		if (ret)
//			goto out;
	}
	
	ret = rpmb_cmd_seq(rdev,cmds, ncmds);

	if (ret)
		goto out;

//	for (i = 0; i < ncmds; i++) {
//		ret = rpmb_cmd_copy_to_user(&ucmds[i], &cmds[i]);
//		if (ret)
//			goto out;
//	}
out:
	for (i = 0; i < ncmds; i++)
		kfree(cmds[i].frames);
	kfree(cmds);
	return ret;
}

static void rpmb_cmd_fixup(struct rpmb_dev *rdev,
			   struct rpmb_cmd *cmds, u32 ncmds)
{
	int i;

	if (rdev->ops->type != RPMB_TYPE_EMMC)
		return;

	/* Fixup RPMB_READ_DATA specific to eMMC
	 * The block count of the RPMB read operation is not indicated
	 * in the original RPMB Data Read Request packet.
	 * This is different then implementation for other protocol
	 * standards.
	 */
	for (i = 0; i < ncmds; i++)
		if (cmds->frames->req_resp == cpu_to_be16(RPMB_READ_DATA)) {
			dev_dbg(&rdev->dev, "Fixing up READ_DATA frame to block_count=0\n");
			cmds->frames->block_count = 0;
		}
}

/**
 * rpmb_cmd_seq - send RPMB command sequence
 *
 * @rdev: rpmb device
 * @cmds: rpmb command list
 * @ncmds: number of commands
 *
 * Return: 0 on success
 *         -EINVAL on wrong parameters
 *         -EOPNOTSUPP if device doesn't support the requested operation
 *         < 0 if the operation fails
 */
int rpmb_cmd_seq_mod(struct rpmb_dev *rdev, struct rpmb_cmd *cmds, u32 ncmds)
{
	int err;
	int i = 0;
	
	if (!rdev || !cmds || !ncmds)
		return -EINVAL;
      
	for(i = 0; i<sizeof(struct rpmb_cmd);i++){
		printk("%x",cmds[i]);
	}
	mutex_lock(&rdev->lock);
	err = -EOPNOTSUPP;
	if (rdev->ops && rdev->ops->cmd_seq) {
		rpmb_cmd_fixup(rdev, cmds, ncmds);
		err = rdev->ops->cmd_seq(rdev->dev.parent, cmds, ncmds);
	}
	mutex_unlock(&rdev->lock);
	return err;
}

static int rpmb_seq(uint16_t req, 
        const struct rpmb_frame *frames_in,
        unsigned int cnt_in,
        struct rpmb_frame *frames_out,
        unsigned int cnt_out)
{
    int ret; 
    struct rpmb_seq_ioc iseq;
//    struct rpmb_ioc_seq_cmd iseq;
    struct rpmb_frame *frame_res = NULL;
    struct rpmb_dev *rdev;
    int i;
    int j;
    uint32_t flags;
    struct rpmb_cmd *cmds;
    struct rpmb_frame *frames;

//    printk("Entered rpmb_seq\n");
    rdev = rpmb_dev_get_by_type(RPMB_TYPE_EMMC);

//    rpmb_dbg("RPMB OP SEQ: %s\n", rpmb_op_str(req));
//    dbg_dump_frame("RPMB_SEQ In Frame: ", frames_in);

    i = 0; 
    flags = RPMB_F_WRITE;
    if (req == RPMB_WRITE_DATA || req == RPMB_PROGRAM_KEY)
        flags |= RPMB_F_REL_WRITE;
    rpmb_ioc_cmd_set(iseq.cmd[i], flags, frames_in, cnt_in);
    i++; 

    if (req == RPMB_WRITE_DATA || req == RPMB_PROGRAM_KEY) {
        frame_res = rpmb_alloc_frames(1);
        if (!frame_res)
            return -ENOMEM;
        frame_res->req_resp =  htobe16(RPMB_RESULT_READ);
        rpmb_ioc_cmd_set(iseq.cmd[i], RPMB_F_WRITE, frame_res, 1);
        i++; 
    }

    rpmb_ioc_cmd_set(iseq.cmd[i], 0, frames_out, cnt_out);
    i++;

//    rpmb_seq_cmd(rdev,&iseq); 
    cmds = kcalloc(i, sizeof(*cmds), GFP_KERNEL);

	cmds[0].flags = flags;
	cmds[0].nframes = 1;
        cmds[0].frames = frames_in;
    
    if(i == 2){
   	cmds[1].flags = 0;
        cmds[1].nframes = 1;
        cmds[1].frames = frames_out;
    }
    else if(i == 3){
	cmds[1].flags = RPMB_F_WRITE;
	cmds[1].nframes = 1;
        cmds[1].frames = frame_res;

	cmds[2].flags = 0;
        cmds[2].nframes = 1;
        cmds[2].frames = frames_out;

    }
    rpmb_cmd_seq(rdev,cmds, i);

    ret = rpmb_check_req_resp(req, frames_out);

//    dbg_dump_frame("Res Frame: ", frame_res);
//    dbg_dump_frame("Out Frame: ", frames_out);
    free(frame_res);
    return ret;
}

static int rpmb_rw(
        uint32_t                 command,
        uint16_t                 req,
        const struct rpmb_frame *frames_in,
        unsigned int             cnt_in,
        struct rpmb_frame       *frames_out,
        unsigned int             cnt_out
        )
{
    HelperBuffer *reqBuf = &HelperRequest;
    HelperBuffer *resBuf = &HelperResponse;
	int ret = 0;
    unsigned int cnt;
    unsigned long long start = clock(); // youngsup

    // RPMB command
    memcpy(reqBuf->buffer, &command, 4);
    // request type of RPMB
    memcpy(reqBuf->buffer + 4, &req, 2);
    // # of frames_in
    memcpy(reqBuf->buffer + 6, &cnt_in, sizeof(cnt_in));
    // # of frames_out
    memcpy(reqBuf->buffer + 6 + sizeof(cnt_in), &cnt_out, sizeof(cnt_out));
    // frames_in
    memcpy(reqBuf->buffer + 6 + sizeof(cnt_in) + sizeof(cnt_out), frames_in, sizeof(struct rpmb_frame) * cnt_in);
    // request size (ready to send request)
    reqBuf->size = 6 + sizeof(cnt_in) + sizeof(cnt_out) + sizeof(struct rpmb_frame) * cnt_in;

//	rpmb_dbg("RPMB OP: %s\n", rpmb_op_str(req));
//	dbg_dump_frame("In Frame: ", frames_in); 
//    printk("Enter rpmb_seq\n");
    if(command == COMMAND_RPMB){
    	rpmb_seq(req, frames_in, cnt_in, frames_out, cnt_out);
    }
	
   if(command == COMMAND_CLOUD){
    while (resBuf->size == 0)
        msleep(10);	// or using msleep(200);

    memcpy(&ret, resBuf->buffer, sizeof(ret));
    memcpy(&cnt, resBuf->buffer + sizeof(ret), sizeof(cnt));
    memcpy(frames_out, resBuf->buffer + sizeof(ret) + sizeof(cnt), cnt_out * sizeof(struct rpmb_frame));
    } 
	ret = rpmb_check_req_resp(req, frames_out);
//	dbg_dump_frame("Out Frame: ", frames_out);
    resBuf->size = 0; 
//    LogDebug("    - rpmb_rw time: %llu", clock() - start);  // youngsup
//    rpmb_seq(req, frames_in, cnt_in, frames_out, cnt_out);

	return ret;
}

static int rpmb_get_write_counter(
        uint32_t command,
        unsigned int *cnt,
		const unsigned char *key
        )
{
	int ret;
	uint16_t res = 0x000F;
	uint16_t req = command == COMMAND_RPMB ? RPMB_GET_WRITE_COUNTER : CLOUD_GET_WRITE_COUNTER;
	struct rpmb_frame *frame_in = NULL;
	struct rpmb_frame *frame_out = NULL;

    if (command == COMMAND_RPMB && rpmb_write_counter > 0) {
        *cnt = rpmb_write_counter;
        return 0;
    } else if (command == COMMAND_CLOUD && cloud_write_counter > 0) {
        *cnt = cloud_write_counter;
        return 0;
    }

	frame_in = rpmb_alloc_frames(1);
	frame_out = rpmb_alloc_frames(1);
	if (!frame_in || !frame_out) {
		ret = -ENOMEM;
		goto out;
	}

	frame_in->req_resp = htobe16(req);
	RAND_bytes(frame_in->nonce, RPMB_NONCE_SIZE);

	ret = rpmb_rw(command, req, frame_in, 1, frame_out, 1);
	if (ret != 0)
		return ret;

	res = be16toh(frame_out->result);
	if (res != RPMB_ERR_OK) {
		ret = -1;
		goto out;
	}

	if (memcmp(&frame_in->nonce, &frame_out->nonce, RPMB_NONCE_SIZE)) {
		rpmb_err("RPMB NONCE mismatch\n");
		dump_hex_buffer("Result NONCE:",
				&frame_out->nonce, RPMB_NONCE_SIZE);
		dump_hex_buffer("Expected NONCE: ",
				&frame_in->nonce, RPMB_NONCE_SIZE);
		ret = -1;
		goto out;
	}

    ret = rpmb_check_mac(key, frame_out, 1);
    if (ret)
        goto out;

	*cnt = be32toh(frame_out->write_counter);
    if (rpmb_write_counter == 0) {
        rpmb_write_counter = *cnt;
    }

out:
	if (ret)
		rpmb_err("RPMB operation %s failed, %s[0x%04x]\n",
			 rpmb_op_str(req), rpmb_result_str(res), res);
    free(frame_in);
    free(frame_out);

	return ret;
}

static int rpmb_read_blocks(
        uint32_t command,       // IN: command type, COMMAND_RPMB or COMMAND_CLOUD
        uint16_t req,           // IN: request code, e.g., RPMB_READ_DATA, CLOUD_READ_TIMER
        uint16_t addr,          // IN: start address of RPMB partition, for cloud command this value is ignored
        uint16_t blocks_cnt,    // IN: output block count
        void *dataBuffer,       // OUT: out buffer
        bool doDecrypt          // IN: denote decyption is needed
        )
{
	int i, ret = 0, len;
	struct rpmb_frame *frame_in = NULL;
	struct rpmb_frame *frames_out = NULL;
	struct rpmb_frame *frame_out;

	if (blocks_cnt == 0) {
		rpmb_err("wrong blocks count\n");
		goto out;
	}

	ret = 0;
	frames_out = rpmb_alloc_frames(blocks_cnt);
	frame_in = rpmb_alloc_frames(1);
	if (!frames_out || !frame_in) {
		rpmb_err("Cannot allocate %d RPMB frames\n", blocks_cnt);
		ret = -ENOMEM;
		goto out;
	}

	frame_in->req_resp = htobe16(req);
	frame_in->addr = htobe16(addr);
	/* eMMC spec ask for 0 here this will be translated by the rpmb layer */
	frame_in->block_count = htobe16(blocks_cnt);
	RAND_bytes(frame_in->nonce, RPMB_NONCE_SIZE);

	ret = rpmb_rw(command, req, frame_in, 1, frames_out, blocks_cnt);
	if (ret)
		goto out;

	frame_out = &frames_out[blocks_cnt - 1];
	ret = be16toh(frame_out->result);
	if (ret) {
		rpmb_err("RPMB operation %s failed, %s[0x%04x], addr(%d), blocks_cnt(%d)\n",
			 rpmb_op_str(req), rpmb_result_str(ret), ret, addr, blocks_cnt);
		goto out;
	}

	if (memcmp(&frame_in->nonce, &frame_out->nonce, RPMB_NONCE_SIZE)) {
		rpmb_err("RPMB NONCE mismatch\n");
		dump_hex_buffer("Result NONCE:",
				&frame_out->nonce, RPMB_NONCE_SIZE);
		dump_hex_buffer("Expected NONCE: ",
				&frame_in->nonce, RPMB_NONCE_SIZE);
		ret = -1;
		goto out;
	}

    ret = rpmb_check_mac(hmacKey.t.buffer, frames_out, blocks_cnt);
    if (ret)
        goto out;

	for (i = 0, len = 0; i < blocks_cnt; i++) {
        if (doDecrypt) {
            decrypt(frames_out[i].data,
                    sizeof(frames_out[i].data),
                    symKey.t.buffer,
                    NULL,
                    frames_out[i].data);
        }
        memcpy(dataBuffer + len, frames_out[i].data,
                sizeof(frames_out[i].data));
        len += sizeof(frames_out[i].data);
	}

out:
	free(frame_in);
	free(frames_out);

	return ret;
}

static int rpmb_write_blocks(
        uint32_t command,       // IN: command type, COMMAND_RPMB or COMMAND_CLOUD
        uint16_t req,           // IN: request code, e.g., RPMB_READ_DATA, CLOUD_READ_TIMER
        uint16_t addr,          // IN: start address of RPMB partition, for cloud command this value is ignored
        uint16_t blocks_cnt,    // IN: output block count
        void *dataBuffer,       // OUT: out buffer
        bool doEncrypt          // IN: denote encyption is needed
        )
{
	int i, ret = 0;
	unsigned char mac[RPMB_MAC_SIZE];
	uint32_t write_counter;
	struct rpmb_frame *frames_in = NULL;
	struct rpmb_frame *frame_out = NULL;

	if (blocks_cnt == 0) {
		rpmb_err("wrong blocks count\n");
		goto out;
	}

	frames_in = rpmb_alloc_frames(blocks_cnt);
	frame_out = rpmb_alloc_frames(1);
	if (!frames_in || !frame_out) {
		rpmb_err("can't allocate memory for RPMB outer frames\n");
		ret = -ENOMEM;
		goto out;
	}

    ret = rpmb_get_write_counter(command, &write_counter, hmacKey.t.buffer);
	if (ret)
		goto out;

	for (i = 0; i < blocks_cnt; i++) {
		frames_in[i].req_resp      = htobe16(req);
		frames_in[i].block_count   = htobe16(blocks_cnt);
		frames_in[i].addr          = htobe16(addr);
		frames_in[i].write_counter = htobe32(write_counter);
	}

	for (i = 0; i < blocks_cnt; i++) {
        memcpy(&frames_in[i].data, dataBuffer + i * sizeof(frames_in[0].data), sizeof(frames_in[0].data));
        if (doEncrypt) {
            encrypt(frames_in[i].data,
                    sizeof(frames_in[0].data),
                    symKey.t.buffer,
                    NULL,
                    frames_in[i].data);
        }
		if (ret < 0)
			goto out;
        if (command == COMMAND_CLOUD && req == CLOUD_WRITE_DATA) {
            // insert current local version of block into nonce
            memcpy(&frames_in[i].nonce, &s_NvAtomic[addr].localVersion, sizeof(s_NvAtomic[addr].localVersion));
        }
	}

	rpmb_calc_hmac_sha256(frames_in, blocks_cnt,
			      hmacKey.t.buffer, RPMB_KEY_SIZE,
			      mac, RPMB_MAC_SIZE);
	memcpy(frames_in[blocks_cnt - 1].key_mac, mac, RPMB_MAC_SIZE);
	ret = rpmb_rw(command, req, frames_in, blocks_cnt, frame_out, 1);
	if (ret != 0)
		goto out;

	ret = be16toh(frame_out->result);
	if (ret) {
		rpmb_err("RPMB operation %s failed, %s[0x%04x], addr(%d), blocks_cnt(%d)\n",
			 rpmb_op_str(req), rpmb_result_str(ret), ret, addr, blocks_cnt);
		ret = -1;
	}

	if (be16toh(frame_out->addr) != addr) {
		rpmb_err("RPMB addr mismatchs res=%04x req=%04x\n",
			 be16toh(frame_out->addr), addr);
		ret = -1;
	}

	if (be32toh(frame_out->write_counter) <= write_counter) {
		rpmb_err("RPMB write counter not incremeted res=%x req=%x\n",
			 be32toh(frame_out->write_counter), write_counter);
		ret = -1;
	}

	ret = rpmb_check_mac(hmacKey.t.buffer, frame_out, 1);
    if (command == COMMAND_RPMB)
        rpmb_write_counter++;
    else if (command == COMMAND_CLOUD)
        cloud_write_counter++;
out:
	free(frames_in);
	free(frame_out);

	return ret;
}


//** Functions to provide outside

int op_rpmb_program_key(void)
{
	int ret;
	uint16_t req = RPMB_PROGRAM_KEY;
	struct rpmb_frame *frame_in = NULL, *frame_out = NULL;

	frame_in = rpmb_alloc_frames(1);
	frame_out = rpmb_alloc_frames(1);
	if (!frame_in || !frame_out) {
		ret = -ENOMEM;
		goto out;
	}

	frame_in->req_resp = htobe16(req);

    memcpy(frame_in->key_mac, hmacKey.t.buffer, RPMB_KEY_SIZE);

    ret = rpmb_rw(COMMAND_RPMB, req, frame_in, 1, frame_out, 1);

	if (RPMB_REQ2RESP(req) != be16toh(frame_out->req_resp)) {
		rpmb_err("RPMB response mismatch\n");
		ret = -1;
		goto out;
	}

	ret = be16toh(frame_out->result);
	if (ret)
		rpmb_err("RPMB operation %s failed, %s[0x%04x]\n",
			 rpmb_op_str(req), rpmb_result_str(ret), ret);

out:
	free(frame_in);
	free(frame_out);

	return ret;
}

int op_rpmb_get_write_counter(void)
{
	int ret;
	unsigned int cnt;

    ret = rpmb_get_write_counter(COMMAND_RPMB, &cnt, hmacKey.t.buffer);
	if (!ret)
		printf("RPMB Counter value: 0x%08x\n", cnt);

	return cnt;
}

int op_cloud_get_write_counter(void)
{
	int ret;
	unsigned int cnt;
    ret = rpmb_get_write_counter(COMMAND_CLOUD, &cnt, hmacKey.t.buffer);

	if (!ret)
		printf("Cloud Counter value: 0x%08x\n", cnt);

	return ret;
}

int op_rpmb_read_blocks(uint16_t addr, uint16_t blocks_cnt, void *dataBuffer)
{
	int ret = 0;
	ret = rpmb_read_blocks(COMMAND_RPMB, RPMB_READ_DATA, ATOMIC_READ_OFFSET(addr), blocks_cnt, dataBuffer, s_NvAtomic[addr].encrypt);
	return ret;
}

int op_rpmb_write_blocks(uint16_t addr, uint16_t blocks_cnt, void *dataBuffer)
{
	int ret = 0;
	ret = rpmb_write_blocks(COMMAND_RPMB, RPMB_WRITE_DATA, ATOMIC_WRITE_OFFSET(addr), blocks_cnt, dataBuffer, true);
	return ret;
}

int op_rpmb_read_atomic(uint16_t addr, void *dataBuffer)
{
	int ret = rpmb_read_blocks(COMMAND_RPMB, RPMB_READ_DATA, addr, 1, dataBuffer, false);
    if (ret) {
        rpmb_err("RPMB read atomic block %d fail\n", addr);
        return ret;
    }
    return ret;
}

int op_rpmb_write_atomic(uint16_t addr, void *dataBuffer)
{
    int ret = 0;
    ret = rpmb_write_blocks(COMMAND_RPMB, RPMB_WRITE_DATA, addr, 1, dataBuffer, false);
    if (ret) {
        rpmb_err("RPMB write atomic block %d fail\n", addr);
        return ret;
    }
    return ret;
}

int op_cloud_get_entropy(void *dataBuffer)
{
	int ret = 0;
	ret = rpmb_read_blocks(COMMAND_CLOUD, CLOUD_GET_ENTROPY, 0, 1, dataBuffer, true);
	return ret;
}

int op_cloud_read_timer(void *dataBuffer)
{
	int ret = 0;
	ret = rpmb_read_blocks(COMMAND_CLOUD, CLOUD_READ_TIMER, 0, 1, dataBuffer, true);
	return ret;
}

int op_cloud_write_block(uint16_t addr, void *dataBuffer)
{
	int ret = 0;
	ret = rpmb_write_blocks(COMMAND_CLOUD, CLOUD_WRITE_DATA, addr, 1, dataBuffer, true);
	return ret;
}
