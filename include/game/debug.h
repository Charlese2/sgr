#ifndef _H_GAME_DEBUG_
#define _H_GAME_DEBUG_

#include "dolphin/types.h"

int DebugError(char* File, u32 Line, char* Error);
int DebugAssert(char* File, u32 Line, char* Assert);

#endif // _H_GAME_DEBUG_
