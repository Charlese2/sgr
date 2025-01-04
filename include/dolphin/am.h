#ifndef _DOLPHIN_AM_H_
#define _DOLPHIN_AM_H_

#include "dolphin.h"
#include "dolphin/ar.h"
#include "dolphin/dvd.h"
#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

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
