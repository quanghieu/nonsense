#ifndef _TPM_HELPER_H
#define _TPM_HELPER_H

#define COMMAND_RPMB                0x1
#define COMMAND_CLOUD               0x2

#define TPM_HELPER_GET_ENTROPY      40
#define TPM_HELPER_NV_READ          41
#define TPM_HELPER_NV_WRITE         42
#define TPM_HELPER_NV_CLEAR         43
#define TPM_HELPER_NV_MOVE          44
#define TPM_HELPER_TIMER_READ       45

#define MAX_BUFFER_SIZE             4096
#define HELPER_MAX_BUFFER           4096
typedef struct {
    unsigned long size;
    unsigned char buffer[HELPER_MAX_BUFFER];
} HelperBuffer;
extern HelperBuffer HelperRequest;
extern HelperBuffer HelperResponse;

#define SYNC_VERSION_THRESHOLD      50

#ifdef HELPER

int init_helper(void);
void cleanup_helper(void);

uint32_t _remote__GetEntropy(void);
uint64_t _cloud__TimerRead(void);
uint32_t _cloud__GetEntropy(void *data);

void _rpmb__NvRead(
        unsigned int     startOffset,   // IN: read start
        unsigned int     size           // IN: size of bytes to read
        );
void _rpmb__NvWrite(
        unsigned int     startOffset,   // IN: write start
        unsigned int     size           // IN: size of bytes to write
        );
void _rpmb__NvAtomicPrepare(
        void *data                      // OUT: data buffer
        );
void _rpmb__NvAtomicCommit(
        void *data                      // IN: data buffer
        );

#else

#define init_helper()
#define cleanup_helper()

#define _remote__GetEntropy()
#define _rpmb__NvRead(startOffset, size)
#define _rpmb__NvWrite(startOffset, size)
#define _rpmb__NvAtomicPrepare(data)
#define _rpmb__NvAtomicCommit(data)

#endif

#endif
