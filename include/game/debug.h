#ifndef _H_GAME_DEBUG_
#define _H_GAME_DEBUG_

#include "dolphin/types.h"

int DebugError(const char* File, u32 Line, const char* Error);
int DebugAssert(const char* File, u32 Line, const char* Assert);

#endif // _H_GAME_DEBUG_
