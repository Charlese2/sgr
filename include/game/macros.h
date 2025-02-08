
#ifndef _H_GAME_MACROS_
#define _H_GAME_MACROS_

#ifdef DEBUG
#define DEBUGASSERTLINE(line, cond) \
    ((cond) || (DebugAssert(__FILE__, line, #cond), 0))

#define DEBUGINT3LINE(line, cond) \
    ((cond) || (DebugError(__FILE__, line, "Int3()")))

#else
#define DEBUGASSERTLINE(line, cond) (void)0
#define DEBUGINT3LINE(line)0
#endif

#define DEBUGERRORLINE(line, error) \
    DebugError(__FILE__, line, error)

#endif // _H_GAME_MACROS_
