#ifndef _DOLPHIN_AM_INTERNAL_H_
#define _DOLPHIN_AM_INTERNAL_H_

#include "dolphin/am.h"

static void __AM_dvd_callback(s32 result, struct DVDFileInfo * handle);
static void __AM_arq_callback(u32 task);
static void __AM_arq_poll_callback(u32 task);
u32 __AMPushBuffered(char * path, void * buffer, u32 offset, u32 buffer_size, void (* callback)(char *), int async_flag);

#endif // _DOLPHIN_AM_INTERNAL_H_
