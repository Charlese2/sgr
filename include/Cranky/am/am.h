#ifndef _DOLPHIN_AM_H_
#define _DOLPHIN_AM_H_

#include "dolphin.h"
#include "dolphin/ar.h"
#include "dolphin/dvd.h"
#include <dolphin/types.h>

typedef void (*AMCallback)(char* path);

void* AMLoadFile(char* path, u32* length);
u32 AMPush(char* path);
u32 AMPushData(void* buffer, u32 length);
void AMPop(void);
u32 AMGetZeroBuffer(void);
u32 AMGetReadStatus(void);
u32 AMGetFreeSize(void);
u32 AMGetStackPointer(void);
void AMInit(u32 aramBase, u32 aramBytes);

#endif
