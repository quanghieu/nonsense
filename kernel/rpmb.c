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
#include <linux/rpmb.h>
#include <linux/slab.h>

#include <linux/debug_rpmb.h>

#include "linux/rpmb.h"

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

#define be16toh __be16_to_cpu
#define be32toh __be32_to_cpu
#define htobe16 __cpu_to_be16
#define htobe32 __cpu_to_be32

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
