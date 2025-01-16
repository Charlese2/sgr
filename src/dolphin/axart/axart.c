#include "dolphin/axart.h"
#include "dolphin/mix.h"
#include "dolphin/os.h"

static AXART_SOUND * __AXARTSoundList;

void AXARTInit() {
    __AXARTSoundList = NULL;
    AXARTSet3DDistanceScale(40.0f);
    AXARTSet3DDopplerScale(20.0);
}

void AXARTQuit() {
    int enabled;
    AXART_SOUND * sound;

    enabled = OSDisableInterrupts();

    for (sound = __AXARTSoundList; sound != NULL; sound = sound->next) {
        MIXReleaseChannel(sound->axvpb);
    }
    __AXARTSoundList = NULL;

    OSRestoreInterrupts(enabled);
}

void AXARTServiceSounds() {
    AXART_SOUND * sound;
    for (sound = __AXARTSoundList; sound != NULL; sound = sound->next) {
        AXARTServiceSound(sound);
    }
}

void AXARTInitSound(AXART_SOUND * sound, AXVPB * voice, u32 sampleRate) {
    sound->articulators = NULL;
    sound->axvpb = voice;
    sound->sampleRate = sampleRate;
}

void AXARTAddSound(AXART_SOUND * sound) {
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

void AXARTRemoveSound(AXART_SOUND * sound) {
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

void AXARTInitArtPanning(AXART_PANNING * articulator) {
    articulator->art.type = 2;
    articulator->pan = 0x40;
    articulator->span = 0x7f;
}

void AXARTInitArtVolume(AXART_VOLUME * articulator) {
    articulator->art.type = 8;
    articulator->attenuation = 0;
}
