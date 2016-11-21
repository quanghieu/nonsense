#ifndef _TPM_STDINT_H
#define _TPM_STDINT_H

#include <linux/types.h>

// from <bits/wordsize.h>
#if defined __x86_64__ && !defined __ILP32__
# define __WORDSIZE 64
#else
# define __WORDSIZE 32
#endif

//#include <asm/config.h>
//#include <compiler.h>

/* Signed.  */
typedef signed char     int8_t;
typedef short int		int16_t;
typedef int			    int32_t;
/*
#if __WORDSIZE == 64
typedef long int		int64_t;
#else
__extension__
typedef long long int		int64_t;
#endif
*/
//
//
//typedef unsigned int __u32;
//
//#if defined(__GNUC__) && !defined(__STRICT_ANSI__)
//#if defined(CONFIG_ARM_32)
//typedef __signed__ long long __s64;
//typedef unsigned long long __u64;
//#elif defined (CONFIG_ARM_64)
//typedef __signed__ long __s64;
//typedef unsigned long __u64;
//#endif
//#endif

/* Unsigned.    */
typedef unsigned char       uint8_t;
typedef unsigned short int	uint16_t;
typedef unsigned int		uint32_t;
/*
#if __WORDSIZE == 64
typedef unsigned long int	uint64_t;
#else
__extension__
typedef unsigned long long int	uint64_t;
#endif
*/

//typedef signed char s8;
//typedef unsigned char u8;
//
//typedef signed short s16;
//typedef unsigned short u16;
//
//typedef signed int s32;
//typedef unsigned int u32;
//
//#if defined(CONFIG_ARM_32)
//typedef signed long long s64;
//typedef unsigned long long u64;
//typedef u32 vaddr_t;
//#define PRIvaddr PRIx32
//typedef u64 paddr_t;
//#define INVALID_PADDR (~0ULL)
//#define PRIpaddr "016llx"
//typedef u32 register_t;
//#define PRIregister "x"
//#elif defined (CONFIG_ARM_64)
//typedef signed long s64;
//typedef unsigned long u64;
//typedef u64 vaddr_t;
//#define PRIvaddr PRIx64
//typedef u64 paddr_t;
//#define INVALID_PADDR (~0UL)
//#define PRIpaddr "016lx"
//typedef u64 register_t;
//#define PRIregister "lx"
//#endif
//
//#if defined(__SIZE_TYPE__)
//typedef __SIZE_TYPE__ size_t;
//#else
//typedef unsigned long size_t;
//#endif
//typedef signed long ssize_t;
//
//typedef char bool_t;
//typedef bool_t bool;
//
//#ifndef NULL
//#define NULL ((void*)0)
//#endif
/* bsd */
//typedef unsigned char           u_char;
//typedef unsigned short          u_short;
//typedef unsigned int            u_int;
//typedef unsigned long           u_long;

/* sysv */
//typedef unsigned char           unchar;
//typedef unsigned short          ushort;
//typedef unsigned int            uint;
//typedef unsigned long           ulong;

//typedef         __u8            uint8_t;
//typedef         __u8            u_int8_t;
//typedef         __s8            int8_t;
//
//typedef         __u16           uint16_t;
//typedef         __u16           u_int16_t;
//typedef         __s16           int16_t;
//
//typedef         __u32           uint32_t;
//typedef         __u32           u_int32_t;
//typedef         __s32           int32_t;
//
//typedef         __u64           uint64_t;
//typedef         __u64           u_int64_t;
//typedef         __s64           int64_t;
//
//
//typedef __u16 __le16;
//typedef __u16 __be16;
//typedef __u32 __le32;
//typedef __u32 __be32;
//typedef __u64 __le64;
//typedef __u64 __be64;

//typedef unsigned long uintptr_t;

#define INT16_MAX               (0x7fff)
#define UINT16_MAX              (65535U)
#define INT32_MAX               (0x7fffffffL)

#define INT_MAX                 ((int)(~0U>>1))
//#define INT_MIN         (-INT_MAX - 1)
//#define UINT_MAX        (~0U)
//#define LONG_MAX        ((long)(~0UL>>1))
//#define LONG_MIN        (-LONG_MAX - 1)
//#define ULONG_MAX       (~0UL)

#endif
