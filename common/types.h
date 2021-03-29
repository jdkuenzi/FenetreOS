#ifndef _TYPES_COMMON_H_
#define _TYPES_COMMON_H_

#ifndef _STDINT_H
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
#endif

#ifndef _STDBOOL_H
typedef unsigned char bool;

#define true   1
#define false  0
#endif

typedef unsigned char uchar_t;
typedef unsigned short ushort_t;
typedef unsigned int uint_t;
typedef unsigned long long ulong_t;

#ifndef NULL
#define NULL 0
#endif

#define FILE_SYSTEM_BUFFER_SIZE 50
typedef struct stat {
    char filename[FILE_SYSTEM_BUFFER_SIZE];
    uint32_t size;
} stat_t;

#define XS_BUFFER 64
#define SM_BUFFER 128
#define MD_BUFFER 256
#define LG_BUFFER 512
#define XL_BUFFER 1024
#define XXL_BUFFER 2048

#define UNUSED(x) ((void)(x))

#endif
