#ifndef _RPMB_H
#define _RPMB_H

#include "PlatformData.h"

#define RPMB_BLOCK_SIZE         256
#define RPMB_BLOCK_OFFSET      ~255

#define CLOUD_READ_TIMER        0x51    /* Read Timer from Cloud */
#define CLOUD_GET_ENTROPY       0x52    /* Get entropy from Cloud */
#define CLOUD_GET_WRITE_COUNTER 0x53    /* Get Cloud write counter */
#define CLOUD_WRITE_DATA        0x54    /* Write RPMB block to Cloud */


#define rpmbBlockNumber(start)      (start / RPMB_BLOCK_SIZE)
#define rpmbBlockOffset(start)      (start & RPMB_BLOCK_OFFSET)
#define rpmbBlockCount_s(size)      ((size + RPMB_BLOCK_SIZE - 1) / RPMB_BLOCK_SIZE)
#define rpmbBlockCount(start, size) rpmbBlockCount_s(start + size - rpmbBlockOffset(start))

#define ATOMIC_BLOCK_COUNT      ((sizeof(s_NvAtomic) + (RPMB_BLOCK_SIZE - 1)) / RPMB_BLOCK_SIZE)
#define ATOMIC_OFFSET(addr, atomic)     \
        (addr + atomic * (NV_MEMORY_SIZE / RPMB_BLOCK_SIZE) + ATOMIC_BLOCK_COUNT)
#define ATOMIC_READ_OFFSET(addr)    ATOMIC_OFFSET(addr, s_NvAtomic[addr].atomic)
#define ATOMIC_WRITE_OFFSET(addr)   ATOMIC_OFFSET(addr, !s_NvAtomic[addr].atomic)


void init_rpmb(bool verbose);

int op_rpmb_program_key(void);
int op_rpmb_get_write_counter(void);
int op_rpmb_read_blocks(uint16_t addr, uint16_t blocks_cnt, void *dataBuffer);
int op_rpmb_write_blocks(uint16_t addr, uint16_t blocks_cnt, void *dataBuffer);
int op_rpmb_read_atomic(uint16_t addr, void *dataBuffer);
int op_rpmb_write_atomic(uint16_t addr, void *dataBuffer);

int op_cloud_read_timer(void *dataBuffer);
int op_cloud_get_entropy(void *dataBuffer);
int op_cloud_get_write_counter(void);
int op_cloud_write_block(uint16_t addr, void *dataBuffer);

#endif
