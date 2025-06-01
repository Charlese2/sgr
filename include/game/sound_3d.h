#ifndef SOUND_3D_H
#define SOUND_3D_H

#include "game/vector.h"
#include "game/timestamp.h"
#include "dolphin/types.h"

class sound_3d {
  public:
    char unk0;
    char unk1;
    u16 unk2;
    s32 state;
    int unk8;
    vector3 unkC;
    vector3 unk18;
    int unk24;
    RealtimeTimer unk28;
    int unk2C;
    GameTimer unk30;
    int unk34;
    int unk38;
    int unk3C;
    int unk40;
    RealtimeTimer unk44;
    RealtimeTimer unk48;
};

void snd_cleanup(unsigned int, unsigned int);
void snd_mark(void);
void snd_release(void);
void snd_setup(void);
void snd_init(void);
void snd_ambient_mask(void);

#endif // SOUND_3D_H
