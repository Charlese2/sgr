#ifndef _H_GAME_NGCSYSTEM_
#define _H_GAME_NGCSYSTEM_

#include "dolphin/gx/GXFifo.h"
#include "dolphin/types.h"

extern u32 Volume_table[21];
extern bool UnknownRenderBool;
extern u8 *pFifoBase[0x4000];
extern GXFifoObj *pFifoObj;
extern bool SystemResetting;

#ifdef DEBUG
#define DriveStatus(first, second) DriveStatusStuff(first)
#else
#define DriveStatus(first, second) DriveStatusStuff(first, second)
#endif

class NGCSystem {
  public:
    static void Reset(int reset, bool forceMenu);
    static void DriveStatus(u8 unk1, int unk2);
    static void InitializeSystems();
    static u32 GetTimeFromTicks(int tick_resolution);
    static void Wait(int milliseconds);
};

#endif // _H_GAME_NGCSYSTEM_
