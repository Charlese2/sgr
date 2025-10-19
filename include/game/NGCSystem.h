#include "dolphin/gx/GXFifo.h"
#include "dolphin/types.h"

extern u32 Volume_table[21];
extern bool UnknownRenderBool;
extern u32 pFifo[16384];

#ifdef DEBUG
#define DriveStatus(first, second) DriveStatusStuff(first)
#else
#define DriveStatus(first, second) DriveStatusStuff(first, second)
#endif

class NGCSystem {
  public:
    static void DriveStatus(u8 unk1, int unk2);
    static void InitializeSystems();
    static u32 GetTicks(int tick_resolution);
};
