#ifndef SOUND_3D_H
#define SOUND_3D_H

#include "game/snd.h"
#include "game/vector.h"
#include "game/timestamp.h"
#include "dolphin/types.h"

class sound_3d {
  public:
    static void set_sound_volume(int type, float volume);
    static void set_music_volume(float volume);
    static void snd_cleanup(u32 unk1, u32 unk2);
    static void snd_mark(void);
    static void snd_release(void);
    static void snd_setup(void);
    static void snd_init(void);
    static void snd_ambient_mask(void);

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

extern class sound_3d SndInstances[MAX_SND_INSTANCES];
extern sound_3d AmbientSounds[MAX_AMBIENT_SOUNDS];

#endif // SOUND_3D_H
