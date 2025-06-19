#include "game/SoundSystem.h"
#include "am/__am.h"
#include "dolphin/sp.h"
#include "game/gamemem.h"
#include "game/vmemory.h"
#include "game/gr.h"
#include "game/FileSystem.h"
#include "game/debug.h"
#include "game/macros.h"
#include "dolphin/axfx.h"
#include "dolphin/ai.h"
#include "dolphin/ar.h"
#include "dolphin/ax.h"
#include "dolphin/dtk.h"
#include "dolphin/dvd.h"
#include "dolphin/mix.h"
#include "dolphin/os.h"
#include "dolphin/types.h"
#include <stdio.h>
#include <string.h>

SoundSystem gSoundSystem;
extern SoundInfo soundInfo;
char buffer[2048];

SoundSystem::SoundSystem() {
    m_buffer       = buffer;
    m_AMZeroBuffer = 0;
    m_curEntry     = 0;
    field14_0xee34 = false;
    m_inUse        = false;
    m_lastPersist  = 0;
#ifndef DEBUG
    m_deativated = FALSE;
#endif
}

void SoundSystem::ProcessSoundsEffects(void) {
    u32 i;
    ambienceEffect *pAmbienceEffect;
    soundEffect *pSoundEffect;

#ifndef DEBUG
    if (!gSoundSystem.IsDeactivated()) {
#endif
        MIXUpdateSettings();
        AXARTServiceSounds();
        if (!gSoundSystem.GetInUse()) {
            gSoundSystem.SetProcessingQueue(true);
            for (i = 0; i < MAX_AMBIANCE_EFFECT_SLOTS; i++) {
                pAmbienceEffect = gSoundSystem.GetAmbianceEffect(i);
                if (!gSoundSystem.GetUnknown2() || pAmbienceEffect->unk42) {
                    AXVPB *pVoice = pAmbienceEffect->m_pVoice;
                    if (pVoice) {
                        if (pVoice->pb.state == 0) {
                            gSoundSystem.RemoveAmbienceEffect(pAmbienceEffect);
                        }
                    }
                }
            }
            if (!gSoundSystem.GetUnknown()) {
                for (i = 0; i < MAX_SOUND_EFFECT_SLOTS; i++) {
                    pSoundEffect  = gSoundSystem.GetSoundEffect(i);
                    AXVPB *pVoice = pSoundEffect->m_pVoice;
                    if (pVoice) {
                        if (pVoice->pb.state == 0) {
                            gSoundSystem.CleanupPlayedSound(i);
                            if (pSoundEffect->index != -1) {
                                pSoundEffect->index2 -= frametime * 1000;
                                if (pSoundEffect->index2 <= 0) {
                                    gSoundSystem.PlaySoundEffect(i);
                                    pSoundEffect->index2 = pSoundEffect->index;
                                }
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

void SoundSystem::RemoveAmbienceEffect(ambienceEffect *sound) {
    AXARTRemoveSound(&sound->m_AXArtSound);
    sound->m_pFile = NULL;
    if (sound->m_pVoice->priority) {
        AXFreeVoice(sound->m_pVoice);
    }
    sound->m_pVoice         = NULL;
    sound->finished_playing = false;
}

void SoundSystem::Initialize() {
    bool lastInUseState = m_inUse;
    m_inUse             = true;
#ifndef DEBUG
    m_deativated = false;
#endif
    field16_0xee36 = false;
    field17_0xee37 = false;
    field14_0xee34 = true;
    ARInit((u32 *)stack_index_addr, 3);
    ARQInit();
    AIInit(0);
    AXInit();
    DTKInit();
    MIXInit();
    AXARTInit();
#ifndef DEBUG
    if (OSGetSoundMode() == 1) {
        soundInfo.special_sound_mode = true;
        AXSetMode(0);
        MIXSetSoundMode(1);
    } else {
        soundInfo.special_sound_mode = false;
        AXSetMode(0);
        MIXSetSoundMode(0);
    }
#else
    AXSetMode(0);
    MIXSetSoundMode(1);
#endif
    InitializeAudio();
    AXRegisterCallback(ProcessSoundsEffects);
    AXFXSetHooks(AllocateReverbMemoryNotImplemented, FreeReverbMemoryNotImplemented);
    AXSetCompressor(1);
    m_inUse = lastInUseState;
}

void *SoundSystem::GetAudioFileSomethingAtIndex(int index) { return m_audio_file[index].unk8; }

void SoundSystem::FreeReverbMemoryNotImplemented(void *address) {
    DebugError("SoundSystem.cpp", 199, "Reverb memory freeing not implemented.  Get Geoff!");
}

void *SoundSystem::AllocateReverbMemoryNotImplemented(u32 unk) {
    DebugError("SoundSystem.cpp", 206, "Reverb memory allocation not implemented.  Get Geoff!");
    return NULL;
}

u32 SoundSystem::GetEntry(char *filename, bool bIsPersistent, bool bFindExisting) {
    bool lastInUseState;
    u32 i;
    audio_file *pFile;

#ifndef DEBUG
    DriveStatus(0, 0);
#endif // !DEBUG

    lastInUseState = m_inUse;
    m_inUse        = true;
    if (bFindExisting) {
        for (i = 0; i < m_curEntry; i++) {
            if (strcmp(m_audio_file[i].file_name, filename) == 0) {
                m_inUse = lastInUseState;
                return i;
            }
        }
    }
    DEBUGASSERTLINE(215, m_curEntry < MAX_SOUNDS);
    if (!bIsPersistent && !m_lastPersist) {
        m_lastPersist = m_curEntry - 1;
    } else {
        DEBUGASSERTLINE(220, (m_lastPersist && !bIsPersistent) || (m_lastPersist == 0 && bIsPersistent));
    }
    pFile = &m_audio_file[m_curEntry];
    DEBUGASSERTLINE(224, strlen(filename) < MAX_SOUND_FILE_NAME_LENGTH);
    strcpy(pFile->file_name, filename);
    pFile->m_pTable = NULL;
    m_inUse         = lastInUseState;
    return m_curEntry++;
}

void SoundSystem::CacheSoundFromDisk() {
    bool lastInUseState;
    bool empty;
    audio_load_cache *test;
    audio_load_cache *pLoad;

    lastInUseState = m_inUse;
    m_inUse        = true;
    empty          = false;
    if (m_CurLoadIndex != -1) {
        audio_load_cache *test = &m_audioLoadcache[m_CurLoadIndex];

        switch (m_audioLoadcache[m_CurLoadIndex].m_pSound->m_LoadStatus) {

        case kLoadJustCompleted:
            empty = true;
            CacheSound(test);
            break;
        default:
            DEBUGASSERTLINE(261, 0);
            break;
        case 2:
        case 3:
            break;
        }
    } else {
        empty = true;
    }

    if (empty) {
        pLoad = GetNextAudioLoadCacheEntry();
        if (pLoad) {
            DEBUGASSERTLINE(275, pLoad->m_pSound->m_LoadStatus == kLoadInitiated);
            LoadAudioFile(pLoad);
        }
    }

    for (int i = 0; i < MAX_PLAY_SLOTS; i++) {
        playSlot *pPlay = &m_PlaySlots[i];
        if (pPlay->m_pInstance) {
            audio_file *pSound = pPlay->m_pInstance->m_pFile;
            DEBUGASSERTLINE(288, pSound);
            if (pSound->m_LoadStatus == kLoadCached) {
                DEBUGASSERTLINE(291, pPlay->m_pInstance);

#ifdef DEBUG
                PlayAmbienceEffect(pPlay->m_pInstance, pPlay->m_LoadStatus, pPlay->m_AXARTVolume, pPlay->unkC, 0, pPlay->unk10);
#endif
                ClearPlaySlot(pPlay);
            }
        }
    }

    m_inUse = lastInUseState;
}

void SoundSystem::DVDReadCallback() {
    audio_load_cache *pLoad;
    sound *pSound;
    u32 sptLength;
    DVDFileInfo *pFileInfo;
    u32 bufferSize;

#ifndef DEBUG
    if (!gSoundSystem.IsDeactivated()) {
#endif

        pLoad            = gSoundSystem.GetCurAudioLodeCacheSlot();
        pSound           = pLoad->m_pSound;
        sptLength        = pSound->m_pTable->entries;
        pSound->m_pTable = (table *)pSound->m_pTable->sound;
        DEBUGASSERTLINE(361, sptLength < MAX_SPT_FILE_SIZE);
        bufferSize = (gSoundSystem.GetDVDFileHandle()->length - sptLength) - 4;
        DVDClose(gSoundSystem.GetDVDFileHandle());
        if (bufferSize > 2048) {
            bufferSize = 2048;
        }
        pSound->m_LoadStatus = kLoadAMPushBuffered;
        pLoad->m_aramStart =
            __AMPushBuffered(pLoad->m_fileName, gSoundSystem.GetBuffer(), sptLength + 4, bufferSize, AMPushBufferedCallback, 1);

#ifndef DEBUG
    }
#endif
}

void SoundSystem::AMPushBufferedCallback(char *path) {
    bool lastInUseState;
    audio_load_cache *pLoad;

    lastInUseState = gSoundSystem.GetInUse();
    gSoundSystem.SetInUse(true);
    DEBUGASSERTLINE(437, gSoundSystem.GetCurLoadIndex() != kNoCurrentLoad);
    pLoad = gSoundSystem.GetCurAudioLodeCacheSlot();
    DEBUGASSERTLINE(441, strcmp(path, pLoad->m_fileName) == 0);
    pLoad->m_pSound->m_LoadStatus = kLoadJustCompleted;
    gSoundSystem.SetInUse(lastInUseState);
}

void SoundSystem::LoadAudioFile(audio_load_cache *pLoad) {
    bool lastInUseState;
    sound *pSound;
    Mempool *pPreviousMempool;
    sound_file_something file;
    int length;

#ifndef DEBUG
    if (!gSoundSystem.IsDeactivated()) {
#endif

        lastInUseState       = m_inUse;
        m_inUse              = true;
        pSound               = pLoad->m_pSound;
        pSound->m_LoadStatus = kLoadStarted;
        strcpy(file.filename, pSound->m_filename);
        length = strlen(file.filename) - 1;
        if (file.filename[length - 4] == '.') {
            file.filename[length - 3] = 'a';
            file.filename[length - 2] = 'u';
            file.filename[length - 1] = 'd';
            file.filename[length]     = '\0';
            length -= 1;
        } else if (file.filename[length - 3] == '.') {
            file.filename[length - 2] = 'a';
            file.filename[length - 1] = 'u';
            file.filename[length]     = 'd';
            file.filename[length + 1] = '\0';
        } else {
            file.filename[length + 1] = '.';
            file.filename[length + 2] = 'a';
            file.filename[length + 3] = 'u';
            file.filename[length + 4] = 'd';
            file.filename[length + 5] = '\0';
            length += 4;
        }
        pPreviousMempool = get_current_mempool();

        set_current_mempool(Gamemem_info.GetSoundMempool());
        pSound->m_pTable = (table *)vmemory::Unknown(128, NULL);
        set_current_mempool(pPreviousMempool);

        sprintf(pLoad->m_fileName, "%s%s", GetFilePath(5), file.filename);
        DVDOpen(pLoad->m_fileName, &m_fileHandle);
        DriveStatus(0, 0);
        DVDReadAsyncPrio(&m_fileHandle, pSound->m_pTable, 128, 0, (DVDCallback)DVDReadCallback, 2);
        m_inUse = lastInUseState;
#ifndef DEBUG
    }
#endif
}

void SoundSystem::LoadUncachedSoundFromDisk() {}

audio_load_cache *SoundSystem::GetNextAudioLoadCacheEntry() {
    bool lastInUseState = m_inUse;
    int nextIndex;
    int index;
    m_inUse = true;
    if (m_CacheQueueRemaining == 0) {
        m_CurLoadIndex = -1;
        m_inUse        = lastInUseState;
        return NULL;
    } else {
        nextIndex = m_CurLoadIndex;
        if (m_CurLoadIndex == -1) {
            nextIndex = 0;
        }
        for (int loadQueueIndex = 0; loadQueueIndex < MAX_SOUND_LOAD_QUEUE_ENTRIES; loadQueueIndex++) {
            index = loadQueueIndex + nextIndex;
            if (index / MAX_SOUND_LOAD_QUEUE_ENTRIES != 0) {
                index %= MAX_SOUND_LOAD_QUEUE_ENTRIES;
            }
            DEBUGASSERTLINE(501, index >= 0 && index <= MAX_SOUND_LOAD_QUEUE_ENTRIES);
            if (m_audioLoadcache[index].m_pSound) {
                m_CurLoadIndex = index;
                m_inUse        = lastInUseState;
                return &m_audioLoadcache[index];
            }
        }
        DEBUGASSERTLINE(511, 0);
        m_inUse = lastInUseState;
        return NULL;
    }
}

void SoundSystem::CacheSound(audio_load_cache *pLoad) {
    sound *pSound;

    bool lastInUseState = m_inUse;
    m_inUse             = true;
    DEBUGASSERTLINE(523, pLoad->m_pSound->m_LoadStatus == kLoadJustCompleted);

    pSound               = pLoad->m_pSound;
    pSound->m_LoadStatus = kLoadCached;
    SPInitSoundTable(pSound->m_pTable, pLoad->m_aramStart, m_AMZeroBuffer);
    InitializeAudioLoadCache(pLoad);
    m_CacheQueueRemaining -= 1;
    m_inUse = lastInUseState;
}

void SoundSystem::PlayAmbienceEffect(ambienceEffect *pAmbienceEffect, s32 pan, float volume, u32 index, bool unk3, bool unk4) {
    bool lastInUseState = m_inUse;

    SOUND_ENTRY *pSound;
    AXVPB *pVoice;
    AXART_SOUND *pAXARTSound;
    AXART_VOLUME *pAXARTVolume;
    AXART_PANNING *pAXARTPanning;

#ifndef DEBUG
    if (GetUnknown()) {
        return;
    }
#endif

    m_inUse                      = true;
    audio_file *pFile            = pAmbienceEffect->m_pFile;
    pAmbienceEffect->index       = index;
    pAmbienceEffect->index2      = index;
    pAmbienceEffect->attenuation = ClampVolume(volume);
    pAmbienceEffect->pan         = ConvertPan(pan);
    if (unk4) {
        pSound = SPGetSoundEntry(pAmbienceEffect->m_pFile->m_pTable, 0);
        DEBUGASSERTLINE(715, pSound);
        pAmbienceEffect->m_pVoice = AXAcquireVoice(15, 0, 0);
        pVoice                    = pAmbienceEffect->m_pVoice;
        DEBUGASSERTLINE(720, pVoice);
        if (!pVoice) {
            m_inUse = lastInUseState;
            return;
        }
        AXSetVoicePriority(pVoice, 5);
        SPPrepareSound(pSound, pVoice, pSound->sampleRate);
        pAXARTSound = &pAmbienceEffect->m_AXArtSound;
        AXARTInitSound(pAXARTSound, pAmbienceEffect->m_pVoice, pSound->sampleRate);
        pAXARTVolume = &pAmbienceEffect->m_AXARTVolume;
        AXARTInitArtVolume(pAXARTVolume);
        pAXARTVolume->attenuation = pAmbienceEffect->attenuation;
        AXARTAddArticulator(pAXARTSound, (AXART_ART *)pAXARTVolume);
        pAXARTPanning = &pAmbienceEffect->m_AXARTPanning;
        AXARTInitArtPanning(pAXARTPanning);
        pAXARTPanning->pan  = pAmbienceEffect->pan;
        pAXARTPanning->span = 127;
        AXARTAddArticulator(pAXARTSound, (AXART_ART *)pAXARTPanning);
        AXARTAddSound(pAXARTSound);
#ifdef DEBUG
        if (unk3) {
            AXSetVoiceState(pVoice, 0);
        } else {
            AXSetVoiceState(pVoice, 1);
        }
#else
        AXSetVoicePriority(pVoice, 1);
#endif
        pAmbienceEffect->finished_playing = true;
    }
    m_inUse = lastInUseState;
}

void SoundSystem::PlaySoundEffect(int index) {
    bool lastInUseState;
    soundEffect *pSoundEffect;
    int i;
    soundEffect *pCurrentSoundEffectSlot;
    SOUND_ENTRY *pSound;
    AXVPB *pVoice;
    AXART_SOUND *pAXARTSound;
    AXART_VOLUME *pAXARTVolume;
    AXART_PANNING *pAXARTPanning;

    if (!field16_0xee36) {
        lastInUseState = m_inUse;
        m_inUse        = true;
        pSoundEffect   = &m_soundEffects[index];
        if (!pSoundEffect->finished_playing && pSoundEffect->m_pFile->m_pTable && pSoundEffect->m_pFile->m_LoadStatus == kLoadCached) {
            for (i = 0; i < MAX_SOUND_EFFECT_SLOTS; i++) {
                pCurrentSoundEffectSlot = &m_soundEffects[i];
                if (pCurrentSoundEffectSlot->m_pFile) {
                    if (strcmp(pSoundEffect->m_pFile->file_name, pCurrentSoundEffectSlot->m_pFile->file_name) == 0) {
                        if (pSoundEffect->finished_playing) {
                            m_inUse = lastInUseState;
                            return;
                        }
                    }
                }
            }
            pSound = SPGetSoundEntry(pSoundEffect->m_pFile->m_pTable, 0);
            DEBUGASSERTLINE(627, pSound);
            pSoundEffect->m_pVoice = AXAcquireVoice(15, 0, 0);
            pVoice                 = pSoundEffect->m_pVoice;
            DEBUGASSERTLINE(632, pVoice);
            if (!pVoice) {
                m_inUse = lastInUseState;
                return;
            }
            AXSetVoicePriority(pVoice, 5);
            SPPrepareSound(pSound, pVoice, pSound->sampleRate);
            pAXARTSound = &pSoundEffect->m_AXArtSound;
            AXARTInitSound(pAXARTSound, pSoundEffect->m_pVoice, pSound->sampleRate);
            pAXARTVolume = &pSoundEffect->m_AXARTVolume;
            AXARTInitArtVolume(pAXARTVolume);
            pAXARTVolume->attenuation = pSoundEffect->attenuation;
            AXARTAddArticulator(pAXARTSound, (AXART_ART *)pAXARTVolume);
            pAXARTPanning = &pSoundEffect->m_AXARTPanning;
            AXARTInitArtPanning(pAXARTPanning);
            pAXARTPanning->pan  = pSoundEffect->pan;
            pAXARTPanning->span = 127;
            AXARTAddArticulator(pAXARTSound, (AXART_ART *)pAXARTPanning);
            AXARTAddSound(pAXARTSound);
            AXSetVoiceState(pVoice, 1);
            pSoundEffect->finished_playing = true;
        }
        m_inUse = lastInUseState;
    }
}

void SoundSystem::CleanupPlayedSound(int index) {
    bool lastInUseState       = m_inUse;
    m_inUse                   = true;
    soundEffect *pSoundEffect = &m_soundEffects[index];
    if (pSoundEffect->finished_playing) {
        AXARTRemoveSound(&pSoundEffect->m_AXArtSound);
        if (pSoundEffect->m_pVoice && pSoundEffect->m_pVoice->priority) {
            AXFreeVoice(pSoundEffect->m_pVoice);
            pSoundEffect->m_pVoice = NULL;
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

    field16_0xee36 = false;
    field17_0xee37 = false;

    for (int i = 0; i < MAX_AMBIANCE_EFFECT_SLOTS; i++) {
    }
    for (int i = 0; i < MAX_SOUND_EFFECT_SLOTS; i++) {
        m_soundEffects[i].index = -1;
    }

    ARFree(&length);
    DEBUGASSERTLINE(1087, length == AUDIO_BLOCK_SIZE_BYTES);
#ifdef NON_MATCHING
    AXARTQuit(); // Needed to prevent prevent null reference exception in sound effect processing after reinitialization.
#endif
    InitializeAudio();
    lastInUseState = m_inUse;
}

void SoundSystem::InitializeAudio(void) {
    bool lastInUseState;
    audio_file *file;

    lastInUseState = m_inUse;
    m_inUse        = TRUE;

    Gamemem_info.GetSoundMempool()->ResetOffset();

    AMInit(ARAlloc(0xc00000), 0xc00000);
    m_AMZeroBuffer = AMGetZeroBuffer();
    memset(m_ambienceEffects, 0, 0x1980);
    memset(m_soundEffects, 0, 0x1100);

    m_CurLoadIndex        = -1;
    m_CacheQueueRemaining = 0;

    for (int i = 0; i < MAX_SOUND_LOAD_QUEUE_ENTRIES; i++) {
        InitializeAudioLoadCache(&m_audioLoadcache[i]);
    }
    for (int i = 0; i < MAX_PLAY_SLOTS; i++) {
        ClearPlaySlot(&m_PlaySlots[i]);
    }
    for (int i = 0; i < MAX_SOUNDS; i++) {
        file               = &m_audio_file[i];
        file->m_pTable     = NULL;
        file->m_LoadStatus = 0;
        if (i > m_lastPersist) {
            file->file_name[0] = '\0';
        }
    }
    if (m_lastPersist != 0) {
        m_curEntry = m_lastPersist + 1;
    } else {
        m_curEntry = 0;
    }
    m_inUse = lastInUseState;
}

playSlot *SoundSystem::GetFreePlaySlot(void) {
    bool lastInUseState;
    lastInUseState = m_inUse;
    for (int i = 0; i < MAX_PLAY_SLOTS; i++) {
        if (m_PlaySlots[i].m_pInstance == NULL) {
            break;
        }
        m_inUse = lastInUseState;
        return &m_PlaySlots[i];
    }
    m_inUse = lastInUseState;
    return (playSlot *)NULL;
}

void SoundSystem::LoadNewSoundsFromDisk(void) {
    bool lastInUseState = m_inUse;
    m_inUse             = true;
    while (m_CacheQueueRemaining) {

        DriveStatus(0, 0);

        CacheSoundFromDisk();
    }
    m_inUse = lastInUseState;
}
