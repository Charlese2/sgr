#include "game/SoundSystem.h"
#include "dolphin/ar.h"
#include "dolphin/types.h"

void InitializeAudio(AudioInterface_Struct * audioBuffers) {
    BOOL previouslyInUse;

    previouslyInUse = audioBuffers->in_use;
    audioBuffers->in_use = TRUE;

    ARAlloc(0xc00000);
    memset(&audioBuffers->field12_0xb1b4, 0, 0x1980);
    memset(&audioBuffers->SomeSFXStruct, 0, 0x1100);

    audioBuffers->next_index = -1;
    audioBuffers->number_in_queue_of_sounds_not_preloaded = 0;

    memset(audioBuffers->audio_cache, 0, 0xdc34);
}
