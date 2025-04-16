
#ifndef _H_GAME_MACROS_
#define _H_GAME_MACROS_

#ifdef DEBUG
#define DEBUGASSERTLINE(line, cond) \
    if (!(cond)) {DebugAssert(__FILE__, line, #cond);}

    #define DEBUGINT3LINE(line, cond) \
    if (!(cond)) {DebugError(__FILE__, line, "Int3()");}

#define DEBUGPRINT(msg, cond) \
    if ((cond)) {printf(msg);}

#else
#define DEBUGASSERTLINE(line, cond) (void)0
#define DEBUGINT3LINE(line, cond) (void)0
#define DEBUGPRINT(line, cond) (void)0
#endif

#define DEBUGERRORLINE(line, error) \
    DebugError(__FILE__, line, error)

#endif // _H_GAME_MACROS_
