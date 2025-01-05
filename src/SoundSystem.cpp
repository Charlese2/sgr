#include "game/SoundSystem.h"
#include "game/debug.h"
#include "dolphin/axfx.h"
#include "dolphin/ai.h"
#include "dolphin/ar.h"
#include "dolphin/ax.h"
#include "dolphin/axart.h"
#include "dolphin/dtk.h"
#include "dolphin/mix.h"
#include "dolphin/os.h"
#include "dolphin/types.h"

extern int DiskStatusStuff(int, int);

SoundSystem soundSystem;
bool lbl_80228FA8;

void SoundSystem::ProcessAXARTSounds(void) {
    if (!soundSystem.deativated){
        MIXUpdateSettings();
        AXARTServiceSounds();
        if (soundSystem.in_use == FALSE){
            soundSystem.processing_queue = TRUE;
        }
        
    }
}

void SoundSystem::Initialize() {
    bool previously_in_use = in_use;
    deativated = false;
    field22_0xee36 = false;
    field23_0xee37 = false;
    field20_0xee34 = true;
    ARInit((u32*)stack_index_addr, 3);
    AXInit();
    ARQInit();
    AIInit((u8*)NULL);
    AXInit();
    DTKInit();
    MIXInit();
    AXARTInit();
    if (OSGetSoundMode()) {
        lbl_80228FA8 = true;
        AXSetMode(0);
        MIXSetSoundMode(1);
    } else {
        lbl_80228FA8 = false;
        AXSetMode(0);
        MIXSetSoundMode(0);
    }
    InitializeAudio();
    AXRegisterCallback(ProcessAXARTSounds);
    AXFXSetHooks(AllocateReverbMemoryNotImplemented, FreeReverbMemoryNotImplemented);
    AXSetCompressor(1);
    in_use = previously_in_use;
}

void SoundSystem::FreeReverbMemoryNotImplemented(void * address) {
    DebugError("SoundSystem.cpp", 199, "Reverb memory freeing not implemented.  Get Geoff!");
}

void * SoundSystem::AllocateReverbMemoryNotImplemented(u32 unk ) {
    DebugError("SoundSystem.cpp", 206, "Reverb memory allocation not implemented.  Get Geoff!");
    return NULL;
}

void SoundSystem::LoadUncachedSoundFromDisk(void) {

}

void SoundSystem::AddSound(int index) {

}

void SoundSystem::ReinitializeAudio(bool unk) {
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

void SoundSystem::InitializeAudio(void) {
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

void SoundSystem::LoadNewSoundsFromDisk() {

}
