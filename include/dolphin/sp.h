#ifndef _DOLPHIN_SP_H_
#define _DOLPHIN_SP_H_

#include "dolphin/ax.h"
#include "dolphin/types.h"

typedef struct {
    // total size: 0x2E
    struct _AXPBADPCM adpcm; // offset 0x0, size 0x28
    struct _AXPBADPCMLOOP adpcmloop; // offset 0x28, size 0x6
} ADPCM;

typedef struct {
    // total size: 0x1C
    u32 type; // offset 0x0, size 0x4
    u32 sampleRate; // offset 0x4, size 0x4
    u32 loopAddr; // offset 0x8, size 0x4
    u32 loopEndAddr; // offset 0xC, size 0x4
    u32 endAddr; // offset 0x10, size 0x4
    u32 currentAddr; // offset 0x14, size 0x4
    ADPCM * adpcm; // offset 0x18, size 0x4
} SOUND_ENTRY;

typedef struct {
    // total size: 0x20
    u32 entries; // offset 0x0, size 0x4
    SOUND_ENTRY sound[1]; // offset 0x4, size 0x1C
} table;

// sp.c
void SPInitSoundTable(table* table, u32 aramBase, u32 zeroBase);
SOUND_ENTRY * SPGetSoundEntry(table* table, u32 index);
void SPPrepareSound(SOUND_ENTRY* sound, AXVPB * axvpb, u32 sampleRate);
void SPPrepareEnd(SOUND_ENTRY* sound, AXVPB* axvpb);

#endif // _DOLPHIN_SP_H_
