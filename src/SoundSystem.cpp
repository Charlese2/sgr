#include "game/SoundSystem.h"
#include "dolphin/am.h"
#include "dolphin/sp.h"
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
    u32 i;
    if (!gSoundSystem.m_deativated){
        MIXUpdateSettings();
        AXARTServiceSounds();
        if (gSoundSystem.GetInUse() == FALSE){
            gSoundSystem.SetProcessingQueue(true);
            for (i = 0; i < 96; i++) {
                
            }

            for (i = 0; i < 64; i++) {
                gSoundSystem.CleanupPlayedSound(i);
            }
        }
        
    }
}

void SoundSystem::Initialize() {
    bool previously_in_use = m_InUse;
    m_InUse = true;
    m_deativated = false;
    field22_0xee36 = false;
    field23_0xee37 = false;
    field20_0xee34 = true;
    ARInit((u32*)stack_index_addr, 3);
    ARQInit();
    AIInit(0);
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
    bool lastInUseState = m_InUse;
    soundEffect* pSoundEffect;
    SOUND_ENTRY* pSound;
    AXVPB* pVoice;

    m_InUse = true;
    pSoundEffect = &m_soundEffect[index];
    if (!pSoundEffect->sound_finished_playing) {
        pSound = SPGetSoundEntry(pSoundEffect->table, index);
        pVoice = AXAcquireVoice(15, 0, 0);
        AXSetVoicePriority(pVoice, 5);
        SPPrepareSound(pSound, pVoice, pSound->sampleRate);
        AXARTInitSound(&pSoundEffect->sound, pSoundEffect->axvpb, pSound->sampleRate);
        AXARTInitArtVolume(&pSoundEffect->volume);
        pSoundEffect->volume.attenuation = pSoundEffect->attenuation;
        AXARTAddArticulator(&pSoundEffect->sound, (AXART_ART*)&pSoundEffect->volume);
        AXARTInitArtPanning(&pSoundEffect->panning);
        pSoundEffect->panning.pan = pSoundEffect->pan;
        pSoundEffect->panning.span = 127;
        AXARTAddArticulator(&pSoundEffect->sound, (AXART_ART*)&pSoundEffect->panning);
        AXARTAddSound(&pSoundEffect->sound);
        AXSetVoicePriority(pVoice, 1);
        pSoundEffect->sound_finished_playing = true;
    }
    m_InUse = lastInUseState;
}

void SoundSystem::CleanupPlayedSound(int index) {
    bool lastInUseState = m_InUse;
    m_InUse = true;
    soundEffect* pSoundEffect = &m_soundEffect[index];
    if (pSoundEffect->sound_finished_playing) {
        AXARTRemoveSound(&pSoundEffect->sound);
        if (pSoundEffect->axvpb && pSoundEffect->axvpb->priority) {
            AXFreeVoice(pSoundEffect->axvpb);
            pSoundEffect->axvpb = 0;
        }
        pSoundEffect->sound_finished_playing = false;
    }
    m_InUse = lastInUseState;
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
    audio_file * file;
    Memory* soundMempool;

    previously_in_use = m_InUse;
    m_InUse = TRUE;

    soundMempool = Gamemem_info.GetSoundMempool();
    ResetOffset(soundMempool);

    aramBase = ARAlloc(0xc00000);
    AMInit(aramBase, 0xc00000);
    m_AMZeroBuffer = AMGetZeroBuffer();
    memset(&m_axVoice, 0, 0x1980);
    memset(m_soundEffect, 0, 0x1100);

    m_next_index = -1;
    m_number_in_queue_of_sounds_not_preloaded = 0;

    for (int i = 0; i < 32; i++) {
        InitializeAudioCache(&m_Audio_cache[i]);
    }
    for (int i = 0; i < 128; i++) {
        InitializeSoundSlot(&m_sound_slots[i]);
    }
    for (int i = 0; i < 1024; i++) {
        file = &m_audio_file[i];
        *(int*)file->file_name = 0;
        *(int*)(file->file_name + 4) = 0;
        if (i > m_lastPersist) {
            file->file_name[0xc] = char(0);
        }
    }
    if (m_lastPersist != 0) {
        m_curEntry = m_lastPersist + 1;
    } else {
        m_curEntry = 0;
    }
    m_InUse = previously_in_use;
}

sound_slot * SoundSystem::GetFreeSoundSlot(void) {
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

void SoundSystem::SetProcessingQueue(bool processing) {
    m_processing_queue = processing;
}

bool SoundSystem::GetInUse(void) {
    return m_InUse;
}

void SoundSystem::InitializeSoundSlot(sound_slot* slot) {
    slot->next = 0;
    slot->prev = 0;
    slot->unk8 = 0.0f;
}

char* SoundSystem::GetBuffer(void) {
    return m_buffer;
}
