/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

//** Description

// This file contains the kernel module to a TPM simulator.
// based on tpm2/Simulator/srcc/TcpServer.c

//** Includes, Locals, Defines and Function Prototypes
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>        /* for put_user */
#include <linux/slab.h>
#include <linux/data_protection.h>

/* TPM 2.0 Engine */
#include "TpmBuildSwitches.h"

#include "Implementation.h"
#include "GpMacros.h"
#include "Capabilities.h"
#include "TpmTypes.h"
#include "Platform_fp.h"

#include "Manufacture_fp.h"
#include "_TPM_Init_fp.h"
#include "Startup_fp.h"

#ifndef __IGNORE_STATE__

//#define MAX_BUFFER 1048576
#define MAX_BUFFER 4096
char OutputBuffer[MAX_BUFFER];      //The output data buffer for the simulator.
char InputBuffer[MAX_BUFFER];       //The input data buffer for the simulator.
struct in_buffer
{
    unsigned long BufferSize;
    unsigned char *Buffer;
} InBuffer;
struct out_buffer
{
    uint32_t      BufferSize;
    unsigned char *Buffer;
} OutBuffer;

#endif // __IGNORE_STATE___

/* Kernel Module info */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Youngsup Oh <youngsup@postech.ac.kr>");
MODULE_DESCRIPTION("Linux Kernel-level Software Trusted Platform Module 2.0 (TPM2.0)");
MODULE_VERSION("dev");
MODULE_SUPPORTED_DEVICE(TPM_DEVICE_ID);

/* project and build version */
#define VERSION_MAJOR       1
#define VERSION_MINOR       24
#define VERSION_BUILD       475

/* TDDL and LKM configuration */
#define TPM_LOG_FILE        "/var/log/tpmd.log"

#define TPM_DEVICE_NAME     "tpm0"
#define TPM_MODULE_NAME     "kswtpm"
#define SUCCESS             0


//** Prototypes - this would normally go in a .h file
static int init_tpm_module(void);
static void cleanup_tpm_module(void);
static int tpm_open(struct inode *, struct file *);
static int tpm_release(struct inode *, struct file *);
static ssize_t tpm_read(struct file *, char *, size_t, loff_t *);
static ssize_t tpm_write(struct file *, const char *, size_t, loff_t *);


//** Global variables are declared as static, so are global within the file.
static int TPM_Open = 0;    /*
                             * Is device open?
                             * Used to prevent multiple access to device
                             */

struct file_operations tpm_fops = {
    .owner          = THIS_MODULE,
    .open           = tpm_open,
    .release        = tpm_release,
    .read           = tpm_read,
    .write          = tpm_write,
//    .unlocked_ioctl = tpm_ioctl,
};

static struct miscdevice tpm_dev = {
    .minor          = MISC_DYNAMIC_MINOR,
    .name           = TPM_DEVICE_NAME,
    .fops           = &tpm_fops,
};


//** Functions

//*** init_tpm_module()
// This function is called when the module is loaded
static int __init
init_tpm_module(void)
{
    Startup_In in;
    int res;

    LogInfo("TPM2 module initialize");

    /* Initilize (ref: tpm2/Simulator/src/TPMCmds.c) */
    // TPM Manufacture
    _plat__NVEnable(NULL);
    if(TPM_Manufacture(1) != 0)
        return 1;
    // Coverage test - repeated manufacturing attempt
    if(TPM_Manufacture(0) != 1)
        return 2;
    // Coverage test - re-manufacturing
    TPM_TearDown();
    if(TPM_Manufacture(1) != 0)
        return 3;
    // Disable NV memory
    _plat__NVDisable();

    /* Powerup (ref: ibmtss356/powerup.c) */
    // Pass power on signal to platform
    _plat__Signal_PowerOn();
    // Turns on the indicator that indicates that NV is available
    _plat__SetNvAvail();
    // Pass power on signal to TPM
    _TPM_Init();

    /* Startup (ref: ibmtss356/startup.c) */
    // Initialize TPM because a system-wide reset
    /*
     * TPM_SU_CLEAR     on TPM2_Startup(), indicates that the TPM should perform TPM Reset or TPM Restart
     * TPM_SU_STATE     on TPM2_Startup(), indicates that the TPM should restore the
     *                  state saved by TPM2_Shutdown(TPM_SU_STATE)
     */
    in.startupType = TPM_SU_CLEAR;
    TPM2_Startup(&in);


    /*
     * Create the "tpm" device in the /sys/class/misc directory.
     * Udev will automatically create the /dev/hello device using
     * the default rules.
     */
    res = misc_register(&tpm_dev);
    if(res != 0) {
        LogError("Unable to register \"TPM\" misc device\n");
        return res;
    }

	/* initialize variables */
    TPM_Open = 0;
    OutBuffer.BufferSize = 0;

    LogInfo("TPM2 module initialization success!");
	return SUCCESS;
}

//*** cleanup_tpm_module()
// This function is called when the module is unloaded
static void __exit
cleanup_tpm_module(void)
{
    int ret;

    TPM_TearDown();
    // Unregist the device
    ret = misc_deregister(&tpm_dev);
    if(ret < 0)
        LogError("Error in misc_deregister: %d", ret);
    LogInfo("Cleaning up TPM2 module.");
}


//*** tpm_open()
// Called when a process tries to open the device file, like
// "cat /dev/tpm0"
static int
tpm_open(struct inode *inode, struct file *file)
{
    LogInfo("TPM2 open!");
    if(TPM_Open)
        return -EBUSY;

    TPM_Open = 1;
    return SUCCESS;
}

//*** tpm_release()
// Called when a process closes the device file.
static int
tpm_release(struct inode *inode, struct file *file)
{
    LogInfo("TPM2 release!");
    TPM_Open = 0;
    return 0;
}

//*** tpm_read()
// Called when a process, which already opened the dev file, attempts to
// read from it.
static ssize_t
tpm_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
    entry_gate();
    LogDebug("Start TPM read");
    if(OutBuffer.BufferSize > 0 ) {
        count = min(count, (size_t)OutBuffer.BufferSize - (size_t)(*ppos));
        count -= copy_to_user(buf, &OutBuffer.Buffer[*ppos], count);
        *ppos += count;
        if((size_t)OutBuffer.BufferSize == (size_t)*ppos) {
            OutBuffer.BufferSize = 0;
        }
    } else {
        count = 0;
    }
    LogDebug("Finish TPM read(%zd)", count);
    exit_gate();
    return count;
}

//*** tpm_write()
// Called when a process write to dev file: echo "hi" > /dev/tpm0
static ssize_t
tpm_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
    entry_gate();
    LogDebug("Start TPM write(%zd)", count);

    *ppos = 0;

    InBuffer.Buffer = (BYTE*)buf;
    InBuffer.BufferSize = count;
    OutBuffer.BufferSize = MAX_BUFFER;
    OutBuffer.Buffer = (unsigned char *)&OutputBuffer;

    // Do implementation-specific command dispatch
    _plat__RunCommand(InBuffer.BufferSize, InBuffer.Buffer,
            &OutBuffer.BufferSize, &OutBuffer.Buffer);
    if(OutBuffer.BufferSize < 0) {
        exit_gate();
        return -EILSEQ;
    }

//    LogDebug("Finish TPM write(%zd)\n", (size_t)tpm_response.size);
    exit_gate();
    return count;
}

module_init(init_tpm_module);
module_exit(cleanup_tpm_module);

