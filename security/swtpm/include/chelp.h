#ifndef _TPM_CHELP_H
#define _TPM_CHELP_H

#include <linux/stddef.h>
#include <stdlib.h>

#ifdef DEBUG_MSG
#define LogInfo(fmt, ...)  printk(KERN_INFO "swtpm %s:%d: Info: " fmt "\n", \
                                __FILE__, __LINE__, ## __VA_ARGS__)
/*
#define LogDebug(fmt, ...) printk(KERN_DEBUG "swtpm %s:%d: Debug: " fmt "\n", \
                                __FILE__, __LINE__, ## __VA_ARGS__)
*/
#define LogDebug(fmt, ...) printk(KERN_DEBUG "swtpm Debug: " fmt "\n", \
                                ## __VA_ARGS__)
#else
#define LogInfo(fmt, ...)
#define LogDebug(fmt, ...)
#endif

#define LogError(fmt, ...) printk(KERN_ERR "swtpm %s:%d: Error: " fmt "\n", \
                                __FILE__, __LINE__, ## __VA_ARGS__)
#define LogAlert(fmt, ...) printk(KERN_ALERT "swtpm %s:%d: Alert: " fmt "\n", \
                                __FILE__, __LINE__, ## __VA_ARGS__)

#ifndef OPENSSL_NO_DEPRECATED
#define OPENSSL_NO_DEPRECATED
#endif  // OPENSSL_NO_DEPRECATED    // youngsup

#ifndef OPENSSL_SMALL_FOOTPRINT
//#define OPENSSL_SMALL_FOOTPRINT
#endif  // OPENSSL_SMALL_FOOTPRINT  // youngsup

#ifndef RSA_CHINESE_REMAINDER_THEOREM
#define RSA_CHINESE_REMAINDER_THEOREM
#endif  // RSA_CHINESE_REMAINDER_THEOREM    // rsa decryption optimization

#ifndef RSA_NO_CONSTTIME
//#define RSA_NO_CONSTTIME
#endif  // RSA_NO_CONSTTIME

// OpenSSL のためのコンパイルフラグ
#ifndef	GETPID_IS_MEANINGLESS
#define	GETPID_IS_MEANINGLESS
#endif	// GETPID_IS_MEANINGLESS

#ifndef	L_ENDIAN
#define	L_ENDIAN
#endif	// L_ENDIAN

#ifndef	_CRT_SECURE_NO_DEPRECATE
#define	_CRT_SECURE_NO_DEPRECATE
#endif	// _CRT_SECURE_NO_DEPRECATE

#ifndef	_CRT_NONSTDC_NO_DEPRECATE
#define	_CRT_NONSTDC_NO_DEPRECATE
#endif	// _CRT_NONSTDC_NO_DEPRECATE

#ifndef	OPENSSL_NO_ENGINE
#define	OPENSSL_NO_ENGINE
#endif	// OPENSSL_NO_ENGINE

#ifndef	OPENSSL_NO_DYNAMIC_ENGINE
#define	OPENSSL_NO_DYNAMIC_ENGINE   // youngsup
#endif	// OPENSSL_NO_DYNAMIC_ENGINE

#ifndef	OPENSSL_NO_CAMELLIA
#define	OPENSSL_NO_CAMELLIA
#endif	// OPENSSL_NO_CAMELLIA

#ifndef	OPENSSL_NO_SEED
#define	OPENSSL_NO_SEED
#endif	// OPENSSL_NO_SEED

#ifndef	OPENSSL_NO_RC5
#define	OPENSSL_NO_RC5
#endif	// OPENSSL_NO_RC5

#ifndef	OPENSSL_NO_MDC2
#define	OPENSSL_NO_MDC2
#endif	// OPENSSL_NO_MDC2

#ifndef	OPENSSL_NO_TLSEXT
#define	OPENSSL_NO_TLSEXT
#endif	// OPENSSL_NO_TLSEXT

#ifndef	OPENSSL_NO_KRB5
#define	OPENSSL_NO_KRB5
#endif	// OPENSSL_NO_KRB5

#ifndef	OPENSSL_NO_SOCK
#define	OPENSSL_NO_SOCK
#endif	// OPENSSL_NO_SOCK

#ifndef	OPENSSL_NO_SSL2
#define	OPENSSL_NO_SSL2
#endif	// OPENSSL_NO_SSL2

#ifndef	OPENSSL_NO_SSL3
#define	OPENSSL_NO_SSL3
#endif	// OPENSSL_NO_SSL3

#ifndef	OPENSSL_NO_HW
#define	OPENSSL_NO_HW
#endif	// OPENSSL_NO_HW

#ifndef	OPENSSL_NO_DGRAM
#define	OPENSSL_NO_DGRAM
#endif	// OPENSSL_NO_DGRAM

#ifndef	OPENSSL_NO_FP_API
#define	OPENSSL_NO_FP_API
#endif	// OPENSSL_NO_FP_API


#ifndef OPENSSL_NO_STDIO
#define OPENSSL_NO_STDIO
#endif  // OPENSSL_NO_STDIO

#ifndef OPENSSL_DISABLE_OLD_DES_SUPPORT
#define OPENSSL_DISABLE_OLD_DES_SUPPORT
#endif  // OPENSSL_DISABLE_OLD_DES_SUPPORT 

#ifndef TERMIO
#define TERMIO
#endif  // TERMIO

#ifndef OPENSSL_NO_GMP
#define OPENSSL_NO_GMP
#endif  // OPENSSL_NO_GMP

#ifndef OPENSSL_NO_JPAKE
#define OPENSSL_NO_JPAKE
#endif  // OPENSSL_NO_JPAKE

#ifndef OPENSSL_NO_MD2
#define OPENSSL_NO_MD2
#endif  // OPENSSL_NO_MD2

#ifndef OPENSSL_NO_RC5
#define OPENSSL_NO_RC5
#endif  // OPENSSL_NO_RC5

#ifndef OPENSSL_NO_RFC3779
#define OPENSSL_NO_RFC3779
#endif  // OPENSSL_NO_RFC3779

#ifndef OPENSSL_NO_STORE
#define OPENSSL_NO_STORE
#endif  // OPENSSL_NO_STORE


/* from swtpm 1.2 */
#define NO_STRINGS_H 1
#define OPENSSL_NO_SYS_TYPES_H 1
#define OPENSSL_NO_SYS_PARAM_H 1
#define THREADS 1
#define NCOMPAT 1
//#define OPENSSL_NO_BIO 1
#define OPENSSL_NO_OCSP 1

#define OPENSSL_NO_ERR

#define BUFSIZ 2048

#define double long
#define printf(format, ...)     printk(KERN_INFO "" format, ## __VA_ARGS__)

#define stderr      NULL
#define vfprintf    BIO_printf
#define DECLARE_PEM_write_fp_const  DECLARE_PEM_write_fp


void *BIO_new_file(const char *filename, const char *mode);

#ifdef CONFIG_SWTPM_PROTECTION
void init_shadow_malloc(void);
#else
void inline init_shadow_malloc(void) { }
#endif

static inline int 
atoi(const char *nptr)
{
    int s, i = 0;
    unsigned int r = 0, m;

    switch (nptr[0]) {
        case '+':
            i++;
        default:
            s = 0;
            m = INT_MAX;
            break;
        case '-':
            i++;
            s = 1;
            m = (unsigned int)INT_MAX + 1U; 
            break;
    }   
    while (nptr[i] >= '0' && nptr[i] <= '9') {
        if (r > m / 10) {
            r = m;
            break;
        }   
        r = r * 10 + (nptr[i] - '0');
        if (r > m) {
            r = m;
            break;
        }   
        i++;
    }   
    if (s) 
        return -r; 
    else
        return r;
}

#endif
