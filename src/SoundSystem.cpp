#include "game/SoundSystem.h"
#include "dolphin/am.h"
#include "dolphin/sp.h"
#include "game/gamemem.h"
#include "game/NGCSystem.h"
#include "game/debug.h"
#include "game/macros.h"
#include "dolphin/axfx.h"
#include "dolphin/ai.h"
#include "dolphin/ar.h"
#include "dolphin/ax.h"
#include "dolphin/dtk.h"
#include "dolphin/mix.h"
#include "dolphin/os.h"
#include "dolphin/types.h"

SoundSystem gSoundSystem;
extern SoundInfo soundInfo;
char buffer[2048];

SoundSystem::SoundSystem() {
    m_buffer       = buffer;
    m_AMZeroBuffer = 0;
    m_curEntry     = 0;
    field20_0xee34 = FALSE;
    m_inUse        = FALSE;
    m_lastPersist  = 0;
#ifndef DEBUG
    m_deativated = FALSE;
#endif
}

void SoundSystem::ProcessSoundsEffects(void) {
    u32 i;

    MIXUpdateSettings();
    AXARTServiceSounds();
#ifndef DEBUG
    if (!gSoundSystem.m_deativated) {
#endif
        if (!gSoundSystem.InUse()) {
            gSoundSystem.SetProcessingQueue(true);
            for (i = 0; i < 96; i++) {
                sound *sound = gSoundSystem.GetSoundEffectSound(i);
                if (gSoundSystem.GetUnknown2() && sound->unk42 || sound->axvpb && sound->axvpb->pb.state == 0) {
                    gSoundSystem.RemoveSound(sound);
                }
            }
            if (!gSoundSystem.GetUnknown()) {
                for (i = 0; i < 64; i++) {
                    soundEffect *sound_effect = gSoundSystem.GetSoundEffect(i);
                    if (sound_effect->axvpb && sound_effect->axvpb->pb.state == 0) {
                        gSoundSystem.CleanupPlayedSound(i);
                        if (sound_effect->index != -1) {
                            if (sound_effect->field7_0x38 < 1) {
                                gSoundSystem.PlaySoundEffect(i);
                                sound_effect->field7_0x38 = sound_effect->index;
                            }
                        }
                    }
                }
            }
            gSoundSystem.SetProcessingQueue(false);
        }
#ifndef DEBUG
    }
#endif
}

void SoundSystem::RemoveSound(sound *sound) {
    AXARTRemoveSound(&sound->axart_sound);
    sound->m_loadStatus = FALSE;
    if (sound->axvpb->priority) {
        AXFreeVoice(sound->axvpb);
    }
    sound->axvpb   = NULL;
    sound->playing = false;
}

void SoundSystem::Initialize() {
    bool previously_in_use = m_inUse;
    m_inUse                = true;
#ifndef DEBUG
    m_deativated = false;
#endif
    field22_0xee36 = false;
    field23_0xee37 = false;
    field20_0xee34 = true;
    ARInit((u32 *)stack_index_addr, 3);
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
    m_inUse = previously_in_use;
}

void SoundSystem::FreeReverbMemoryNotImplemented(void *address) {
    DebugError("SoundSystem.cpp", 199, "Reverb memory freeing not implemented.  Get Geoff!");
}

void *SoundSystem::AllocateReverbMemoryNotImplemented(u32 unk) {
    DebugError("SoundSystem.cpp", 206, "Reverb memory allocation not implemented.  Get Geoff!");
    return NULL;
}

void SoundSystem::LoadSoundFromDisk() {
    bool previously_in_use = m_inUse;
    bool result;
    m_inUse = true;
    result  = false;
    if (m_next_index == -1) {
        result = true;
    } else {
    }
    DEBUGASSERTLINE(261, 0);
    audio_load_cache *pLoad = GetNextAudioLoadCacheEntry();
    DEBUGASSERTLINE(275, pLoad->m_pSound->m_loadStatus == kLoadInitiated);
}

void SoundSystem::LoadUncachedSoundFromDisk() {}

audio_load_cache *SoundSystem::GetNextAudioLoadCacheEntry() {
    bool previously_in_use = m_inUse;
    int next_index;
    int index;
    m_inUse = true;
    if (m_number_in_queue_of_sounds_not_preloaded == 0) {
        m_next_index = -1;
        m_inUse      = previously_in_use;
        return NULL;
    } else {
        next_index = m_next_index;
        if (next_index == -1) {
            next_index = 0;
        }
        for (int loadQueueIndex = 0; loadQueueIndex < 32; loadQueueIndex++) {
            index = loadQueueIndex + next_index;
        }
        if (index == -1) {
            index = 0;
        }
        DEBUGASSERTLINE(501, index >= 0 && index <= MAX_SOUND_LOAD_QUEUE_ENTRIES);
        if (m_audioLoadcache[index].m_pSound) {
            m_next_index = index;
            m_inUse      = previously_in_use;
            return &m_audioLoadcache[index];
        }
        DEBUGASSERTLINE(511, 0);
        m_inUse = previously_in_use;
        return NULL;
    }
}

void SoundSystem::AddSound(int index) {
    bool lastInUseState = m_inUse;
    soundEffect *pSoundEffect;
    SOUND_ENTRY *pSound;
    AXVPB *pVoice;

    if (field22_0xee36) {
        return;
    }

    m_inUse      = true;
    pSoundEffect = &m_soundEffects[index];
    if (!pSoundEffect->finished_playing) {
        pSound = SPGetSoundEntry(pSoundEffect->table, index);
        DEBUGASSERTLINE(715, pSound);
        pVoice = AXAcquireVoice(15, 0, 0);
        DEBUGASSERTLINE(720, pVoice);
        AXSetVoicePriority(pVoice, 5);
        SPPrepareSound(pSound, pVoice, pSound->sampleRate);
        AXARTInitSound(&pSoundEffect->sound, pSoundEffect->axvpb, pSound->sampleRate);
        AXARTInitArtVolume(&pSoundEffect->volume);
        pSoundEffect->volume.attenuation = pSoundEffect->attenuation;
        AXARTAddArticulator(&pSoundEffect->sound, (AXART_ART *)&pSoundEffect->volume);
        AXARTInitArtPanning(&pSoundEffect->panning);
        pSoundEffect->panning.pan  = pSoundEffect->pan;
        pSoundEffect->panning.span = 127;
        AXARTAddArticulator(&pSoundEffect->sound, (AXART_ART *)&pSoundEffect->panning);
        AXARTAddSound(&pSoundEffect->sound);
        AXSetVoicePriority(pVoice, 1);
        pSoundEffect->finished_playing = true;
    }
    m_inUse = lastInUseState;
}

void SoundSystem::PlaySoundEffect(int index) {
    bool lastInUseState;
    if (!field22_0xee36) {
        lastInUseState = m_inUse;
    }
}

void SoundSystem::CleanupPlayedSound(int index) {
    bool lastInUseState       = m_inUse;
    m_inUse                   = true;
    soundEffect *pSoundEffect = &m_soundEffects[index];
    if (pSoundEffect->finished_playing) {
        AXARTRemoveSound(&pSoundEffect->sound);
        if (pSoundEffect->axvpb && pSoundEffect->axvpb->priority) {
            AXFreeVoice(pSoundEffect->axvpb);
            pSoundEffect->axvpb = 0;
        }
        pSoundEffect->finished_playing = false;
    }
    m_inUse = lastInUseState;
}

void SoundSystem::ReinitializeAudio(bool loadNewSounds) {
    u32 length;
    BOOL lastInUseState = m_inUse;

    while (m_processing_queue) {
    }
    if (loadNewSounds) {
        LoadNewSoundsFromDisk();
    }

    field22_0xee36 = false;
    field23_0xee37 = false;

    for (int i = 0; i < 96; i++) {
    }
    for (int i = 0; i < 64; i++) {
        m_soundEffects[i].index = -1;
    }

    ARFree(&length);
    DEBUGASSERTLINE(1087, length == AUDIO_BLOCK_SIZE_BYTES);
    InitializeAudio();
    lastInUseState = m_inUse;
}

void SoundSystem::InitializeAudio(void) {
    bool previously_in_use;
    audio_file *file;

    previously_in_use = m_inUse;
    m_inUse           = TRUE;

    Gamemem_info.GetSoundMempool()->ResetOffset();

    AMInit(ARAlloc(0xc00000), 0xc00000);
    m_AMZeroBuffer = AMGetZeroBuffer();
    memset(m_ambienceEffects, 0, 0x1980);
    memset(m_soundEffects, 0, 0x1100);

    m_next_index                              = -1;
    m_number_in_queue_of_sounds_not_preloaded = 0;

    for (int i = 0; i < 32; i++) {
        InitializeAudioLoadCache(&m_audioLoadcache[i]);
    }
    for (int i = 0; i < 128; i++) {
        InitializeSoundSlot(&m_sound_slots[i]);
    }
    for (int i = 0; i < 1024; i++) {
        file       = &m_audio_file[i];
        file->unk0 = 0;
        file->unk4 = 0;
        if (i > m_lastPersist) {
            file->file_name[0] = '\0';
        }
    }
    if (m_lastPersist != 0) {
        m_curEntry = m_lastPersist + 1;
    } else {
        m_curEntry = 0;
    }
    m_inUse = previously_in_use;
}

sound_slot *SoundSystem::GetFreeSoundSlot(void) {
    bool previously_in_use;
    previously_in_use = m_inUse;
    for (int i = 0; i < 128; i++) {
        if (m_sound_slots[i].next == NULL) {
            break;
        }
        m_inUse = previously_in_use;
        return m_sound_slots + i;
    }
    m_inUse = previously_in_use;
    return (sound_slot *)NULL;
}

void SoundSystem::LoadNewSoundsFromDisk(void) {
    bool previously_in_use = m_inUse;
    m_inUse                = true;
    while (m_number_in_queue_of_sounds_not_preloaded) {

        DriveStatus(0, 0);

        LoadSoundFromDisk();
    }
    m_inUse = previously_in_use;
}
