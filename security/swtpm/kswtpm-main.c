#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>        
#include <asm/uaccess.h>        /* for put_user */
#include <linux/slab.h>
#include <helper.h>

// for block device (NV cache)
#include <linux/fs.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>

#ifdef CONFIG_SWTPM_PROTECTION
#include <linux/data_protection.h>
#include <linux/nv.h>
#else
#undef entry_gate
#undef exit_gate
#define entry_gate()
#define exit_gate()
#endif


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

#include "PlatformData.h"
#include "Tpm.h"

#ifndef __IGNORE_STATE__

//#define MAX_BUFFER 1048576
#define MAX_BUFFER 4096
char OutputBuffer[MAX_BUFFER];      //The output data buffer for the simulator.
char InputBuffer[MAX_BUFFER];       //The input data buffer for the simulator.
struct {
    uint32_t      BufferSize;
    unsigned char *Buffer;
} InBuffer, OutBuffer;

extern void __attribute__((section(".mct"))) (*NvOps) (uint64_t);
uint64_t new_clockVal = 0;
uint64_t old_clockVal = 0;
uint64_t start_clock = 0;
//extern unsigned long __attribute__((section(".mct"))) addr_tpmTime;

unsigned int ORDER_DATA = NV_ORDERLY_DATA;
EXPORT_SYMBOL(ORDER_DATA);
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

static int TPM_Init = 0;    /*
                             * Is tpm initialized?
                             */

struct file_operations tpm_fops = {
    .owner          = THIS_MODULE,
    .open           = tpm_open,
    .release        = tpm_release,
    .read           = tpm_read,
    .write          = tpm_write,
};

static struct miscdevice tpm_dev = {
    .minor          = MISC_DYNAMIC_MINOR,
    .name           = TPM_DEVICE_NAME,
    .fops           = &tpm_fops,
};


//** Functions
static void clockUpdate(uint64_t unused){
//	pr_info("Start clock update\n");
	NvClock(&new_clockVal, &old_clockVal, start_clock);
//	pr_info("End clock update\n");
}

static int
init_tpm(void)
{
    Startup_In in;

    LogInfo("TPM2 initialize");

    /* Initilize (ref: tpm2/Simulator/src/TPMCmds.c) */
    // TPM Manufacture
//    _plat__NVEnable(NULL);
    if(TPM_Manufacture(1) != 0)
        return 1;
    // Disable NV memory
//    _plat__NVDisable();

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

    NvRead(&go, NV_ORDERLY_DATA, sizeof(go));
//    printk("Count is %llx\n",go.clock);
    start_clock = go.clock;
//    NvOps = &clockUpdate;
//    NvOps(go.clock);

    TPM_Init = 1;
    LogInfo("TPM2 initialize finish!! go.cloudClock(%llu)", go.cloudClock);
//    NvOps = &NvClock;
//    printk("[ kdp ] Before write to RPMB\n");
//    NvOps(0x123456);
//    printk("[ kdp ] After write to RPMB\n");


    return 0;
}



//*** init_tpm_module()
// This function is called when the module is loaded
static int __init
init_tpm_module(void)
{
    int res;
    unsigned long shadow_flags = 0;
    unsigned long val = 0x0;
    LogInfo("TPM2 module initialize");
    s_moduleInit = false;
    init_helper();
#ifdef CONFIG_SWTPM_PROTECTION
    init_shadow_malloc();
#endif

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
    NvRead(&go, NV_ORDERLY_DATA, sizeof(go));
    printk("Count is %llx\n",go.clock);
//    start_clock = go.clock;
//    NvOps = &clockUpdate;
//    NvOps(go.clock);

	/* initialize variables */
    TPM_Open = 0;
    OutBuffer.BufferSize = 0;

    pr_info("TPM2 module initialization success!\n");

#ifdef HELPER
    s_moduleInit = true;
#endif
/*    pr_info("tpmTime address = %px",&s_tpmTime);
#ifdef CONFIG_SWTPM_PROTECTION
    shadow_flags = shadow_entry_gate();
//    addr_tpmTime = (unsigned long)&go.clock; 
    pr_info("Enable TPM\n");
//    _plat__NvAtomicPrepare();
    exit_gate();
#endif */
	return SUCCESS;
}

//*** cleanup_tpm_module()
// This function is called when the module is unloaded
static void __exit
cleanup_tpm_module(void)
{
    TPM_TearDown();
    // Unregist the device
    misc_deregister(&tpm_dev);
    cleanup_helper();
    LogInfo("Cleaning up TPM2 module.");
}


//*** tpm_open()
// Called when a process tries to open the device file, like
// "cat /dev/tpm0"
static int
tpm_open(struct inode *inode, struct file *file)
{
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
    exit_gate();
    return count;
}

//*** tpm_write()
// Called when a process write to dev file: echo "hi" > /dev/tpm0
static ssize_t
tpm_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
    int ret;
    entry_gate();

    *ppos = 0;
    InBuffer.Buffer = (BYTE*)&InputBuffer;
    InBuffer.BufferSize = count;
    OutBuffer.BufferSize = MAX_BUFFER;
    OutBuffer.Buffer = (unsigned char *)&OutputBuffer;

    
    ret = copy_from_user(InputBuffer, buf, count);
    // Do implementation-specific command dispatch
    _plat__NvAtomicPrepare();
//    NvOps = &clockUpdate;
//    NvOps(0x12345);
    if (!TPM_Init){
        init_tpm();
        printk("Init TPM\n");
    }
       
    _plat__RunCommand(InBuffer.BufferSize, InBuffer.Buffer,
            &OutBuffer.BufferSize, &OutBuffer.Buffer);
    if(OutBuffer.BufferSize < 0) {
        exit_gate();
        return -EILSEQ;
    }

    exit_gate();
    return count;
}

module_init(init_tpm_module);
module_exit(cleanup_tpm_module);

