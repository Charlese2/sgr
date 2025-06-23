#include "game/sound_3d.h"
#include "game/matrix.h"
#include "game/console.h"
#include "game/SoundConvert.h"
#include "game/vector.h"
#include "game/macros.h"

u8 soundEffectsSetup;
int snd_marks;
int disableSoundEffects;

vector3 position1;
vector3 position2;
vector3 position3;

console_command call_sound_effect_init("snd_init", "", CALL, (CommandCallbackInt)sound_3d::snd_init);
console_command call_ambient_mask_toggle("snd_ambient_mask", "", CALL, (CommandCallbackInt)sound_3d::snd_ambient_mask);

void sound_3d::set_sound_volume(int type, float volume) {
    if (!disableSoundEffects) {
        if (type < 0 || type > 3) {
            DEBUGINT3LINE(478);
            return;
        }

    }
}

void sound_3d::snd_cleanup(u32 unk1, u32 unk2) {
    if (disableSoundEffects) {
        return;
    }
}

void sound_3d::snd_mark() {
    if (disableSoundEffects) {
        return;
    }
#ifdef DEBUG
    printf("SND: =======> snd_mark()\n");
#endif
    snd_marks += 1;
}

void sound_3d::snd_release() {
    if (disableSoundEffects || snd_marks == 0) {
        return;
    }
#ifdef DEBUG
    printf("SND: <======= snd_release()\n");
#endif
    snd_marks -= 1;
    SoundConvert::ReinitializeSoundSystems();
    for (int i = 0; i < 2048; i++) {
    }
    for (int i = 0; i < MAX_SND_INSTANCES; i++) {
    }
    for (int i = 0; i < MAX_AMBIENT_SOUNDS; i++) {
    }
}

void sound_3d::snd_setup() {
    int i;
    if (disableSoundEffects) {
        return;
    }
    for (i = 0; i < MAX_SND_INSTANCES; i++) {
        SoundConvert::SetupSoundEffect(&SndInstances[i]);
        SndInstances[i].state &= 0xFF;
    }
    for (i = 0; i < MAX_AMBIENT_SOUNDS; i++) {
        SoundConvert::SetupSoundEffect(&AmbientSounds[i]);
        AmbientSounds[i].state &= 0xFF;
    }
    if (!soundEffectsSetup) {
        for (i = 0; i < 2048; i++) {
        }
        for (i = 0; i < 4; i++) {
        }
        snd_marks = 0;
    }
    SetVector3(0.0f, 0.0f, 0.0f, &position1);
    SetVector3(-1.0f, 0.0f, 0.0f, &position2);
    SoundConvert::InitializeSoundSystem();
    if (soundEffectsSetup) {
    } else {
    }
    soundEffectsSetup = true;
}

u8 IsSoundEffectsSetup() { return soundEffectsSetup; }

void sound_3d::snd_init() { snd_setup(); }

void sound_3d::snd_ambient_mask() {}
