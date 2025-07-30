#include "dolphin/gx/GXFifo.h"
#include "dolphin/types.h"

extern u32 Volume_table[21];
extern bool UnknownRenderBool;
extern u32 pFifo[16384];

class NGCSystem {
  public:
    static void InitializeSystems();
};

#ifdef DEBUG
#define DriveStatus(first, second) DriveStatusStuff(first)
#else
#define DriveStatus(first, second) DriveStatusStuff(first, second)
#endif

void InitializeNGCSystems();
#ifdef DEBUG
void DriveStatusStuff(u8 unk1);
#else
void DriveStatusStuff(u8 unk1, int unk2);
#endif
