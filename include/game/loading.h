#include "dolphin/types.h"

class loading {
  public:
    static void PostRetraceCallback(u32 retraceCount);
    static void start_loadscreen(void);
    static void stop_loadscreen(void);
    static void pause_loadscreen(void);
    static void unpause_loadscreen(void);
};
