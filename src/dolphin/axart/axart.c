#include "dolphin/axart.h"
#include "dolphin/mix.h"
#include "dolphin/os.h"
#include "dolphin/types.h"

static AXARTSound * AXVPBQueue;

void AXARTServiceSounds() {
    AXARTSound * p;
    for (p = AXVPBQueue; p != NULL; p = p->next) {
        AXARTServiceSound(p);
    }
}

void AXARTInitSound(AXARTSound* sound, AXVPB* axvpb, float sampleRate) {
    sound->unk10 = NULL;
    sound->axvpb = axvpb;
    sound->sampleRate = sampleRate;
}

void AXARTAddSound(AXARTSound * sound) {
    BOOL enabled;

    AXSetVoiceItdOn(sound->axvpb);
    MIXInitChannel(sound->axvpb, 0, -0x388, -0x388, -0x388, 64, 64, 0);
    enabled = OSDisableInterrupts();

    if (AXVPBQueue) {
        AXVPBQueue->prev = sound;
        sound->next = AXVPBQueue;
    } else {
        sound->next = NULL;
    }
    sound->prev = NULL;
    AXVPBQueue = sound;
    OSRestoreInterrupts(enabled);
}

void AXARTRemoveSound(AXARTSound* sound) {
    BOOL enabled;
    AXVPB *next, *prev;

    enabled = OSDisableInterrupts();
    if (sound == (void*)AXVPBQueue) {
        AXVPBQueue = sound->next;
        if (AXVPBQueue != NULL) {
            AXVPBQueue->prev = NULL;
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
