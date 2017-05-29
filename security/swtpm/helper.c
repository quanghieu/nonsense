#include <helper.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/uaccess.h>        /* for put_user */
#include "rpmb.h"
#include "PlatformData.h"

#define HELPER_DEVICE_NAME  "helper"

static int helper_open(struct inode *, struct file *); 
static int helper_release(struct inode *, struct file *); 
static ssize_t helper_read(struct file *, char *, size_t, loff_t *); 
static ssize_t helper_write(struct file *, const char *, size_t, loff_t *); 

static int Helper_Open = 0;

HelperBuffer HelperRequest;
HelperBuffer HelperResponse;

struct file_operations helper_fops = {
    .owner          = THIS_MODULE,
    .open           = helper_open,
    .release        = helper_release,
    .read           = helper_read,
    .write          = helper_write
};

static struct miscdevice helper_dev = {
    .minor          = MISC_DYNAMIC_MINOR,
    .name           = HELPER_DEVICE_NAME,
    .fops           = &helper_fops
};

#define RPMB_BLOCK_COUNT    (NV_MEMORY_SIZE / RPMB_BLOCK_SIZE)
static uint8_t cachedNV[RPMB_BLOCK_COUNT];
static uint8_t updateNV[RPMB_BLOCK_COUNT];
static uint8_t updateAtomicNV[ATOMIC_BLOCK_COUNT];

//*** init_helper()
// Initialize helper module
int init_helper(void)
{
    int res;

    res = misc_register(&helper_dev);
    if(res != 0) {
        LogError("Unable to register \"HERLPER\" misc device\n");
        return res;
    }

    Helper_Open = 0;
    HelperRequest.size = 0;
    HelperResponse.size = 0;

    // generate Symmetric Key and HMAC key from cloud seed
    init_rpmb(false);
    memset(cachedNV, 0, sizeof(cachedNV));
    memset(updateNV, 0, sizeof(updateNV));
    /*
    // set RPMB HMAC key
    op_rpmb_program_key(&HelperRequest, &HelperResponse);
    // get RPMB write counter
    op_rpmb_get_write_counter(&HelperRequest, &HelperResponse);
    */

    pr_info("Helper module initialization success!\n");

    return 0;
}

//*** cleanup_helper()
// Cleanup helper module
void cleanup_helper(void)
{
    misc_deregister(&helper_dev);
}

//*** helper_open()
// Called when a process tries to open the device file, like
// "cat /dev/helper"
static int
helper_open(struct inode *inode, struct file *file)
{
    if(Helper_Open)
        return -EBUSY;
    Helper_Open = 1;
    return 0;
}

//*** helper_release()
// Called when a process closes the device file.
static int
helper_release(struct inode *inode, struct file *file)
{
    Helper_Open = 0;
    return 0;
}

//*** helper_read()
// Called when a process, which already opened the dev file, attempts to
// read from it.
static ssize_t
helper_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
    while (1) {
        if(HelperRequest.size > 0) {
            count = min(count, (size_t)HelperRequest.size - (size_t)(*ppos));
            count -= copy_to_user(buf, &HelperRequest.buffer[*ppos], count);
            *ppos += count;
            if((size_t)HelperRequest.size == (size_t)*ppos) {
                HelperRequest.size = 0;
                *ppos = 0;
            }
            break;
        }
//        return 0;
        msleep(20);
    }
    return count;
}

//*** helper_write()
// Called when a process write to dev file: echo "hi" > /dev/helper
static ssize_t
helper_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
    int ret;
    *ppos = 0;

    ret = copy_from_user(HelperResponse.buffer, buf, count);
    HelperResponse.size = count;

    return count;
}


//** Helper functions and values for RPMB
// 

void _rpmb__NvRead(
        unsigned int     startOffset,   // IN: read start offset
        unsigned int     size           // IN: size of bytes to read
        )
{
    int ret = 0, i;
    uint16_t blockNumber = rpmbBlockNumber(startOffset);
    uint16_t blockCount = rpmbBlockCount(startOffset, size);

    for (i = 0; i < blockCount; i++) {
        if (cachedNV[blockNumber + i]) // youngsup: do not use read cache
            continue;

        LogDebug("_rpmb__NvRead: startOffset(%d), size(%d), blockNumber(%d), blockCount(%d)",
                startOffset, size, blockNumber, blockCount);

        ret = op_rpmb_read_blocks(blockNumber + i, 1, &s_NV[(blockNumber + i) * RPMB_BLOCK_SIZE]);
        if (ret) {
            LogError("RPMB read fail");
            return;
        }
        cachedNV[blockNumber + i] = 1;
    }
}

void _rpmb__NvWrite(
        unsigned int     startOffset,   // IN: write start offset
        unsigned int     size           // IN: size of bytes to write
        )
{
    uint32_t i;
    uint16_t blockNumber = rpmbBlockNumber(startOffset);
    uint16_t blockCount = rpmbBlockCount(startOffset, size);

    LogDebug("_rpmb__NvWrite: startOffset(%d), size(%d), blockNumber(%d), blockCount(%d)",
            startOffset, size, blockNumber, blockCount);

    for (i = 0; i < blockCount; i++)
        updateNV[blockNumber + i] = 1;
}

void _rpmb__NvAtomicPrepare(
        void *data                      // OUT: data buffer
        )
{
    int ret, i;
    LogDebug("%s", __FUNCTION__);
    for (i = 0; i < ATOMIC_BLOCK_COUNT; i++) {
        ret = op_rpmb_read_atomic(i, &s_NvAtomic[i * RPMB_BLOCK_SIZE]);
        if (ret) {
            LogError("RPMB read fail");
            return;
        }
    }
    LogDebug("%s finish", __FUNCTION__);
}

void _rpmb__NvAtomicCommit(
        void *data                      // IN: data buffer
        )
{
    int ret, i;
    pr_info("Entered update atomic\n");
    // Write updated NV block
    for (i = 0; i < (NV_MEMORY_SIZE / RPMB_BLOCK_SIZE); i++) {
        if (!updateNV[i])
            continue;

        ret = op_rpmb_write_blocks(i, 1, &s_NV[i * RPMB_BLOCK_SIZE]);
        if (ret) {
            LogError("RPMB write fail");
            return;
        }
	pr_info("Update 1 done\n");
        s_NvAtomic[i].atomic = !s_NvAtomic[i].atomic;
        s_NvAtomic[i].encrypt = true;
        s_NvAtomic[i].localVersion++;
        cachedNV[i] = 1;
        updateNV[i] = 0;

        LogDebug("NVWrite: localVersion: %d, commitVersion: %d", s_NvAtomic[i].localVersion, s_NvAtomic[i].commitVersion);
        if (s_NvAtomic[i].localVersion - s_NvAtomic[i].commitVersion > SYNC_VERSION_THRESHOLD) {
            LogDebug("  - This block synced too late. need to sync with cloud");
//            ret = op_cloud_write_block(i, &s_NV[i * RPMB_BLOCK_SIZE]);
//            if (ret) {
//                LogError("Cloud sync error");
//            } else {
//                s_NvAtomic[i].commitVersion = s_NvAtomic[i].localVersion;
//            }
        }

        updateAtomicNV[i / (RPMB_BLOCK_COUNT / ATOMIC_BLOCK_COUNT)] = 1;
    }

    // Write updated Atomic block
    for (i = 0; i < ATOMIC_BLOCK_COUNT; i++) {
        if (!updateAtomicNV[i])
            continue;

        LogDebug("%s (block idx: %d)", __FUNCTION__, i);
        ret = op_rpmb_write_atomic(i, &s_NvAtomic[i * RPMB_BLOCK_SIZE]);
        if (ret) {
            LogError("RPMB write fail");
            return;
        }
	pr_info("Update 2 done\n");
        updateAtomicNV[i] = 0;
    }
}


//** Helper functions and values for CLOUD
// 

uint32_t _cloud__GetEntropy(void *data)
{
    int ret = 0;

    ret = op_cloud_get_entropy(data);
    if (ret) {
        LogError("RPMB read fail");
        return 0;
    }

    return RPMB_BLOCK_SIZE / 4;
}

uint64_t _cloud__TimerRead(void)
{
    int ret = 0;
    unsigned char buffer[RPMB_BLOCK_SIZE];
    uint64_t value = 0;

    ret = op_cloud_read_timer(buffer);
    if (ret) {
        LogError("RPMB read fail");
        return 0;
    }
    memcpy(&value, buffer, sizeof(value));

    return value;
}
