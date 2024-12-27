#include "dolphin/axart.h"
#include "dolphin/mix.h"

static AXARTSound * __AXARTSoundList;

void AXARTServiceSounds() {
    AXARTSound * sound;
    for (sound = __AXARTSoundList; sound != NULL; sound = sound->next) {
        AXARTServiceSound(sound);
    }
}

void AXARTInitSound(AXARTSound* sound, AXVPB* axvpb, float sampleRate) {
    sound->articulators = NULL;
    sound->axvpb = axvpb;
    sound->sampleRate = sampleRate;
}

void AXARTAddSound(AXARTSound * sound) {
    BOOL enabled;

    AXSetVoiceItdOn(sound->axvpb);
    MIXInitChannel(sound->axvpb, 0, -0x388, -0x388, -0x388, 64, 64, 0);
    enabled = OSDisableInterrupts();

    if (__AXARTSoundList) {
        __AXARTSoundList->prev = sound;
        sound->next = __AXARTSoundList;
    } else {
        sound->next = NULL;
    }
    sound->prev = NULL;
    __AXARTSoundList = sound;
    OSRestoreInterrupts(enabled);
}

void AXARTRemoveSound(AXARTSound* sound) {
    BOOL enabled;
    AXVPB *next, *prev;

    enabled = OSDisableInterrupts();
    if (sound == (void*)__AXARTSoundList) {
        __AXARTSoundList = sound->next;
        if (__AXARTSoundList != NULL) {
            __AXARTSoundList->prev = NULL;
        }
    } else {
        next = sound->next;
        prev = sound->prev;
        prev->next = next;
        if (next != NULL) {
            next->prev = prev;
        }
    }
    OSRestoreInterrupts(enabled);
    MIXReleaseChannel(sound->axvpb);
}
