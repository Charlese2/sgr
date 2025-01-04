#ifndef _DOLPHIN_AM_H_
#define _DOLPHIN_AM_H_

#include "dolphin.h"
#include "dolphin/ar.h"
#include "dolphin/dvd.h"
#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_STACKS 256

struct {
        // total size: 0x80
        struct DVDFileInfo file_handle; // offset 0x0, size 0x3C
        struct ARQRequest arq_handle; // offset 0x3C, size 0x20
        void (* callback)(char *); // offset 0x5C, size 0x4
        char * path; // offset 0x60, size 0x4
        void * buffer; // offset 0x64, size 0x4
        u32 file_length; // offset 0x68, size 0x4
        u32 curr_read_offset; // offset 0x6C, size 0x4
        u32 read_length; // offset 0x70, size 0x4
        u32 aram_start_addr; // offset 0x74, size 0x4
        u32 curr_aram_offset; // offset 0x78, size 0x4
        int poll_flag; // offset 0x7C, size 0x4
} __AMReadInfo[MAX_STACKS];

void * AMLoadFile(char * path, u32 * length);
u32 AMPush(char * path);
u32 AMPushData(void * buffer, u32 length);
u32 __AMPushBuffered(char * path, void * buffer, u32 offset, u32 buffer_size, void (* callback)(char *), int async_flag);
void AMPop();
u32 AMGetZeroBuffer();
u32 AMGetReadStatus();
u32 AMGetFreeSize();
u32 AMGetStackPointer();
void AMInit(u32 aramBase, u32 aramBytes);

#ifdef __cplusplus
}
#endif 

#endif
