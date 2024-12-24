#include "game/SoundSystem.h"
#include "dolphin/ar.h"
#include "dolphin/types.h"



void SoundSystem::LoadNewSoundsFromDisk() {

}

void SoundSystem::InitializeAudio() {
    BOOL previouslyInUse;

    previouslyInUse = this->in_use;
    this->in_use = TRUE;

    ARAlloc(0xc00000);
    memset(&this->field12_0xb1b4, 0, 0x1980);
    memset(&this->SomeSFXStruct, 0, 0x1100);

    this->next_index = -1;
    this->number_in_queue_of_sounds_not_preloaded = 0;

    memset(this->audio_cache, 0, 0xdc34);
}

void SoundSystem::LoadUncachedSoundFromDisk() {

}

SoundSystem::SoundSystem(){
    
}

void ProcessAXQueue() {

}
