
#ifndef _H_GAME_MACROS_
#define _H_GAME_MACROS_

#ifdef DEBUG
#define DEBUGASSERTLINE(line, cond) \
    ((cond) || (DebugAssert(__FILE__, line, #cond), 0))

#else
#define DEBUGASSERTLINE(line, cond) (void)0
#endif

#endif // _H_GAME_MACROS_
