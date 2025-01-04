#ifndef _DOLPHIN_OSREBOOT_H_
#define _DOLPHIN_OSREBOOT_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

static struct {
    // total size: 0x20
    char date[16]; // offset 0x0, size 0x10
    unsigned long entry; // offset 0x10, size 0x4
    unsigned long size; // offset 0x14, size 0x4
    unsigned long rebootSize; // offset 0x18, size 0x4
    unsigned long reserved2; // offset 0x1C, size 0x4
} Header; // size: 0x20, address: 0x0

static void Run(void * entryPoint);
static void ReadApploader(void * addr, s32 length, s32 offset);
static void Callback();
void OSSetSaveRegion(void * start, void * end);
void OSGetSaveRegion(void * start, void * end);

#ifdef __cplusplus
}
#endif

#endif
