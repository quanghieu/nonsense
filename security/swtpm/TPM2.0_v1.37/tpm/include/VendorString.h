/*(Copyright)
 *      Microsoft Copyright 2009 - 2016
 *      All rights reserved.
 */

#ifndef     _VENDOR_STRING_H
#define     _VENDOR_STRING_H

// Define up to 4-byte values for MANUFACTURER.  This value defines the response
// for TPM_PT_MANUFACTURER in TPM2_GetCapability.
// The following line should be un-commented and a vendor specific string
// should be provided here.
//#define    MANUFACTURER    "MSFT"

// The following #if macro may be deleted after a proper MANUFACTURER is provided.
#ifndef MANUFACTURER
#error MANUFACTURER is not provided. \
Please modify include\VendorString.h to provide a specific \
manufacturer name.
#endif

// Define up to 4, 4-octet, vendor-specific values. The values must each 
// be 4 octet long and the last
// value used may contain trailing zeros.
// These values define the response for TPM_PT_VENDOR_STRING_(1-4)
// in TPM2_GetCapability.
// The following line should be un-commented and a vendor specific string
//
// A vendor-specific string should be provided here.
//#define       VENDOR_STRING_1       "DPA "
//#define       VENDOR_STRING_2       "fTPM"

// The vendor strings 2-4 may also be defined as needed.
//#define       VENDOR_STRING_3
//#define       VENDOR_STRING_4

// The following #if macro may be deleted after a proper VENDOR_STRING_1
// is provided.
#ifndef VENDOR_STRING_1
#error VENDOR_STRING_1 is not provided. \
Please modify include\VendorString.h to provide a vendor specific \
string.
#endif

// A vendor-specific FIRMWARE_V1 is required here. It is
// the more significant 32-bits of a vendor-specific value
// indicating the version of the firmware
//#define   FIRMWARE_V1         (0x20160511)

// A vendor-specific FIRMWARE_V2 may be provided here. If present, it is the less 
// significant 32-bits of the version of the firmware. 
//#define   FIRMWARE_V2         (0x00162800)

// The following macro is just to insure that a FIRMWARE_V1 value is provided.
#ifndef FIRMWARE_V1
#error  FIRMWARE_V1 is not provided. \
Please modify include\VendorString.h to provide a vendor-specific firmware \
version
#endif

#endif
