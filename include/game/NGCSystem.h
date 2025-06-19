#include "dolphin/types.h"

extern u32 Volume_table[21];

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
