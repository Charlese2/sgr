#include "game/SoundSystem.h"
#include "dolphin/ar.h"
#include "dolphin/axart.h"
#include "dolphin/mix.h"
#include "dolphin/os.h"
#include "dolphin/types.h"

extern int DiskStatusStuff(int, int);
extern AXARTSound* AXVPBQueue;


SoundSystem __SoundSystem;

void SoundSystem::LoadNewSoundsFromDisk() {

}

void SoundSystem::InitializeAudio() {
    BOOL previouslyInUse;

    previouslyInUse = in_use;
    in_use = TRUE;

    ARAlloc(0xc00000);
    memset(&axVoice, 0, 0x1980);
    memset(unkcb34, 0, 0x1100);

    next_index = -1;
    number_in_queue_of_sounds_not_preloaded = 0;

    memset(audio_cache, 0, 0xdc34);

    for (int i = 0; i < 26; i++) {
        audio_cache[i].field0_0x0 = 0;
        audio_cache[i].field1_0x4 = 0;
        audio_cache[i].field2_0x8[0] = 0;
    }
}

void SoundSystem::Reinitialize(bool unk) {
    BOOL lastInUseState = in_use;

    while (processing_queue) {}
    if (unk) {
        BOOL lastInUseState2 = in_use;
        in_use = TRUE;
            
        while (number_in_queue_of_sounds_not_preloaded) {
            DiskStatusStuff(0,0);
            LoadNewSoundsFromDisk();
        }

        in_use = lastInUseState;
    }

    field22_0xee36 = false;
    field23_0xee37 = false;

    for (int i = 0; i < 0x60; i++) {
        BOOL lastInUseState3 = in_use;

    }
}

void SoundSystem::AddSound(int index) {

}

void SoundSystem::LoadUncachedSoundFromDisk() {

}

void SoundSystem::ProcessAXARTSounds() {
    if (!__SoundSystem.deativated){
        MIXUpdateSettings();
        AXARTServiceSounds();
        if (__SoundSystem.in_use == FALSE){
            __SoundSystem.processing_queue = TRUE;
        }
        
    }
}
