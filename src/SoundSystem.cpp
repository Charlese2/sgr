#include "game/SoundSystem.h"
#include "dolphin/am.h"
#include "game/gamemem.h"
#include "game/debug.h"
#include "dolphin/axfx.h"
#include "dolphin/ai.h"
#include "dolphin/ar.h"
#include "dolphin/ax.h"
#include "dolphin/dtk.h"
#include "dolphin/mix.h"
#include "dolphin/os.h"
#include "dolphin/types.h"


extern int DiskStatusStuff(int, int);

SoundSystem gSoundSystem;
extern SoundInfo soundInfo;
extern GameMem gamemem;
char buffer[2048];

void SoundSystem::InitializeGlobal() {
    gSoundSystem.m_buffer = buffer;
    gSoundSystem.m_AMZeroBuffer = 0;
    gSoundSystem.m_curEntry = 0;
    gSoundSystem.field20_0xee34 = FALSE;
    gSoundSystem.m_InUse = FALSE;
    gSoundSystem.m_lastPersist = 0;
    gSoundSystem.m_deativated = FALSE;
}

void SoundSystem::ProcessSoundsEffects(void) {
    if (!gSoundSystem.m_deativated){
        MIXUpdateSettings();
        AXARTServiceSounds();
        if (gSoundSystem.m_InUse == FALSE){
            gSoundSystem.m_processing_queue = TRUE;
        }
        
    }
}

void SoundSystem::Initialize() {
    bool previously_in_use = m_InUse;
    m_deativated = false;
    field22_0xee36 = false;
    field23_0xee37 = false;
    field20_0xee34 = true;
    ARInit((u32*)stack_index_addr, 3);
    ARQInit();
    AIInit((u8*)NULL);
    AXInit();
    DTKInit();
    MIXInit();
    AXARTInit();
    if (OSGetSoundMode() == 1) {
        soundInfo.special_sound_mode = true;
        AXSetMode(0);
        MIXSetSoundMode(1);
    } else {
        soundInfo.special_sound_mode = false;
        AXSetMode(0);
        MIXSetSoundMode(0);
    }
    InitializeAudio();
    AXRegisterCallback(ProcessSoundsEffects);
    AXFXSetHooks(AllocateReverbMemoryNotImplemented, FreeReverbMemoryNotImplemented);
    AXSetCompressor(1);
    m_InUse = previously_in_use;
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
    BOOL lastInUseState = m_InUse;

    while (m_processing_queue) {}
    if (unk) {
        BOOL lastInUseState2 = m_InUse;
        m_InUse = TRUE;
            
        while (m_number_in_queue_of_sounds_not_preloaded) {
            DiskStatusStuff(0,0);
            LoadNewSoundsFromDisk();
        }

        m_InUse = lastInUseState;
    }

    field22_0xee36 = false;
    field23_0xee37 = false;

    for (int i = 0; i < 0x60; i++) {
        BOOL lastInUseState3 = m_InUse;

    }
}

void SoundSystem::InitializeAudio(void) {
    bool previously_in_use;
    u32 aramBase;
    audio_file_names * file;

    previously_in_use = m_InUse;
    m_InUse = TRUE;

    aramBase = ARAlloc(0xc00000);
    AMInit(aramBase, 0xc00000);
    m_AMZeroBuffer = AMGetZeroBuffer();
    memset(&m_axVoice, 0, 0x1980);
    memset(m_AXART_Stuff, 0, 0x1100);

    m_next_index = -1;
    m_number_in_queue_of_sounds_not_preloaded = 0;

    memset(m_Audio_cache, 0, 0xdc34);

    for (int i = 0; i < 32; i++) {
        InitializeAudioCache(&m_Audio_cache[i]);
    }
    for (int i = 0; i < 128; i++) {
        InitializeSoundSlot(&m_sound_slots[i]);
    }
    for (int i = 0; i < 1024; i++) {
        file = m_audio_file_names + i;
        *(char *)file->file_name = 0;
        *(char *)(file->file_name + 4) = 0;
        if (i > m_lastPersist) {
            file->file_name[0xc] = char(0);
        }
    }
    if (m_lastPersist == 0) {
        m_curEntry = 0;
    } else {
        m_curEntry = m_lastPersist + 1;
    }
    m_InUse = previously_in_use;
}

sound_slot * SoundSystem::getFreeSoundSlot(void) {
    bool previously_in_use;
    previously_in_use = m_InUse;
    for (int i = 0; i < 128; i++) {
        if (m_sound_slots[i].next == NULL) {
            break;
        }
        m_InUse = previously_in_use;
        return m_sound_slots + i;
    }
    m_InUse = previously_in_use;
    return (sound_slot*)NULL;
}

void SoundSystem::LoadNewSoundsFromDisk(void) {

}

void SoundSystem::InitializeAudioCache(AudioCache* cache) {
    cache->field0_0x0 = 0;
    cache->field1_0x4 = 0;
    cache->m_fileName[0] = (char)0;
}

void SoundSystem::setProcessingQueue(bool processing) {
    m_processing_queue = processing;
}

bool SoundSystem::getInUse(void) {
    return m_InUse;
}

void SoundSystem::InitializeSoundSlot(sound_slot* slot) {
    slot->next = 0;
    slot->prev = 0;
    slot->unk8 = 0.0f;
}

char* SoundSystem::getBuffer(void) {
    return m_buffer;
}
