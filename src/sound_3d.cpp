#include "game/sound_3d.h"
#include "game/matrix.h"
#include "game/console.h"
#include "game/SoundConvert.h"
#include "game/vector.h"

u8 soundEffectsSetup;
int snd_marks;
int disableSoundEffects;

vector3 position1;
vector3 position2;
vector3 position3;

console_command call_sound_effect_init("snd_init", "", CALL, (CommandCallbackInt)snd_init);
console_command call_ambient_mask_toggle("snd_ambient_mask", "", CALL, (CommandCallbackInt)snd_ambient_mask);

void snd_cleanup(u32 unk1, u32 unk2) {
    if (disableSoundEffects) {
        return;
    }
}

void snd_mark() {
    if (disableSoundEffects) {
        return;
    }
#ifdef DEBUG
    printf("SND: =======> snd_mark()\n");
#endif
    snd_marks += 1;
}

void snd_release() {
    if (disableSoundEffects || snd_marks == 0) {
        return;
    }
#ifdef DEBUG
    printf("SND: <======= snd_release()\n");
#endif
    snd_marks -= 1;
    ReinitializeSoundSystems();
    for (int i = 0; i < 2048; i++) {
    }
    for (int i = 0; i < MAX_SND_INSTANCES; i++) {
    }
    for (int i = 0; i < MAX_AMBIENT_SOUNDS; i++) {
    }
}

void snd_setup() {
    int i;
    if (disableSoundEffects) {
        return;
    }
    for (i = 0; i < MAX_SND_INSTANCES; i++) {
        SetupSoundEffect(&SndInstances[i]);
        SndInstances[i].state &= 0xFF;
    }
    for (i = 0; i < MAX_AMBIENT_SOUNDS; i++) {
        SetupSoundEffect(&AmbientSounds[i]);
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
    InitializeSoundSystem();
    if (soundEffectsSetup) {
    } else {
    }
    soundEffectsSetup = true;
}

u8 IsSoundEffectsSetup() { return soundEffectsSetup; }

void snd_init() { snd_setup(); }

void snd_ambient_mask() {}
