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
    m_deactivated = FALSE;
#endif
}

void SoundSystem::ProcessSounds(void) {
    u32 i;
    snd_instance *pSndInstance;
    ambient_sound *pAmbientSound;

#ifndef DEBUG
    if (!gSoundSystem.m_deactivated) {
#endif
        MIXUpdateSettings();
        AXARTServiceSounds();
        if (!gSoundSystem.GetInUse()) {
            gSoundSystem.SetProcessingQueue(true);
            for (i = 0; i < MAX_SND_INSTANCES; i++) {
#ifdef DEBUG
                pSndInstance = gSoundSystem.GetSndInstance(i);
#else
            pSndInstance = &gSoundSystem.m_sndInstances[i];
#endif
                if (!gSoundSystem.GetUnknown2() || pSndInstance->unk42) {
                    AXVPB *pVoice = pSndInstance->m_pVoice;
                    if (pVoice) {
                        if (pVoice->pb.state == 0) {
#ifndef DEBUG
                            if (!gSoundSystem.m_deactivated) {
#endif
                                gSoundSystem.RemoveSndInstance(pSndInstance);
#ifndef DEBUG
                            }
#endif
                        }
                    }
                }
            }
            if (!gSoundSystem.GetUnknown()) {
                for (i = 0; i < MAX_AMBIENT_SOUNDS; i++) {
#ifdef DEBUG
                    pAmbientSound = gSoundSystem.GetAmbientSound(i);
#else
                pAmbientSound = &gSoundSystem.m_ambientSounds[i];
#endif
                    AXVPB *pVoice = pAmbientSound->m_pVoice;
                    if (pVoice) {
                        if (pVoice->pb.state == 0) {
                            gSoundSystem.CleanupPlayedAmbientSound(i);
                            if (pAmbientSound->index != -1) {
                                pAmbientSound->index2 -= frametime * 1000;
                                if (pAmbientSound->index2 <= 0) {
                                    gSoundSystem.PlayAmbientSound(i);
                                    pAmbientSound->index2 = pAmbientSound->index;
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

sound_header *SoundSystem::GetAudioFileSoundHeader(int index) { return m_audio_file[index].m_pSoundHeader; }

void SoundSystem::RemoveSndInstance(snd_instance *sound) {
    AXARTRemoveSound(&sound->m_AXArtSound);
    sound->m_pSound = NULL;
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
    m_deactivated = false;
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
    AXRegisterCallback(ProcessSounds);
    AXFXSetHooks(AllocateReverbMemoryNotImplemented, FreeReverbMemoryNotImplemented);
    AXSetCompressor(1);
    m_inUse = lastInUseState;
}

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
    sound_file *pFile;

#ifndef DEBUG
    DriveStatus(0, 0);
#endif

    lastInUseState = m_inUse;
    m_inUse        = true;
    if (bFindExisting) {
        for (i = 0; i < m_curEntry; i++) {
            if (strcmp(m_audio_file[i].m_filename, filename) == 0) {
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
    strcpy(pFile->m_filename, filename);
    pFile->m_pSoundData = NULL;
    m_inUse             = lastInUseState;
    return m_curEntry++;
}

void SoundSystem::CacheSoundFromDisk() {
    bool lastInUseState;
    bool loadNext;
    sound_load *pLoad;

#ifndef DEBUG
    if (!IsDeactivated()) {
#endif

        lastInUseState = m_inUse;
        m_inUse        = true;
        loadNext       = false;
        if (m_CurLoadIndex != -1) {
            sound_load *pLoad = &m_audioLoadQueue[m_CurLoadIndex];

            switch (m_audioLoadQueue[m_CurLoadIndex].m_pSound->m_loadStatus) {

            case kLoadJustCompleted:
                loadNext = true;
                InitializeSoundTable(pLoad);
                break;
            default:
                DEBUGASSERTLINE(261, 0);
                break;
            case kLoadStarted:
            case kLoadRead:
                break;
            }
        } else {
            loadNext = true;
        }

        if (loadNext) {
            pLoad = GetNextAudioLoadQueueEntry();
            if (pLoad) {
                DEBUGASSERTLINE(275, pLoad->m_pSound->m_loadStatus == kLoadInitiated);
                LoadSoundFile(pLoad);
            }
        }

        for (int i = 0; i < MAX_PLAY_SLOTS; i++) {
            play_slot *pPlay = &m_PlaySlots[i];
            if (pPlay->m_pInstance) {
                sound_file *pSound = pPlay->m_pInstance->m_pSound;
                DEBUGASSERTLINE(288, pSound);
                if (pSound->m_loadStatus == kLoadReady) {
                    DEBUGASSERTLINE(291, pPlay->m_pInstance);

                    PlaySndInstance(pPlay->m_pInstance, pPlay->m_LoadStatus, pPlay->m_AXARTVolume, pPlay->unkC, 0, pPlay->unk10);
                    ClearPlaySlot(pPlay);
                }
            }
        }

        m_inUse = lastInUseState;

#ifndef DEBUG
    }
#endif
}

sound_load *SoundSystem::AddToLoadQueue(int index) {
    bool lastInUseState;
    sound_file *pSound;
    sound_load *pLoad;

#ifndef DEBUG
    if (!IsDeactivated()) {
#endif

        lastInUseState         = m_inUse;
        m_inUse                = true;
        pSound                 = &m_audio_file[index];
        pSound->m_pSoundHeader = GetSoundHeader(pSound->m_filename);
        if (pSound->m_loadStatus == kLoadNotStarted) {
            pLoad = GetNextFreeAudioLoadQueueEntry();
            DEBUGASSERTLINE(318, pLoad);
            pLoad->m_pSound      = pSound;
            pLoad->m_aramStart   = 0;
            pSound->m_loadStatus = kLoadInitiated;
            m_LoadQueueEntries += 1;
            m_inUse = lastInUseState;
            return pLoad;
        } else if (pSound->m_loadStatus == kLoadReady) {
            m_inUse = lastInUseState;
            return NULL;
        } else {
            for (int i = 0; i < MAX_SOUND_LOAD_QUEUE_ENTRIES; i++) {
                if (m_audioLoadQueue[i].m_pSound == pSound) {
                    m_inUse = lastInUseState;
                    return &m_audioLoadQueue[i];
                }
            }
        }
        DEBUGASSERTLINE(345, 0);
        m_inUse = lastInUseState;
        return NULL;

#ifndef DEBUG
    }
#endif
}

void SoundSystem::DVDReadCallback() {
    sound_load *pLoad;
    sound_file *pSound;
    table *pSoundData;
    u32 sptLength;
    u32 bufferSize;

#ifndef DEBUG
    if (!gSoundSystem.IsDeactivated()) {
#endif

        pLoad                = gSoundSystem.GetCurAudioLodeQueueSlot();
        pSound               = pLoad->m_pSound;
        sptLength            = pSound->m_pSoundData->entries;
        pSound->m_pSoundData = (table *)pSound->m_pSoundData->sound;
        DEBUGASSERTLINE(361, sptLength < MAX_SPT_FILE_SIZE);
        bufferSize = (gSoundSystem.GetDVDFileHandle()->length - sptLength) - 4;
        DVDClose(gSoundSystem.GetDVDFileHandle());
        if (bufferSize > 2048) {
            bufferSize = 2048;
        }
        pSound->m_loadStatus = kLoadRead;
        pLoad->m_aramStart =
            __AMPushBuffered(pLoad->m_fileName, gSoundSystem.GetBuffer(), sptLength + 4, bufferSize, AMPushBufferedCallback, 1);

#ifndef DEBUG
    }
#endif
}

void SoundSystem::LoadSoundFile(sound_load *pLoad) {
    bool lastInUseState;
    sound_file *pSound;
    Mempool *pPreviousMempool;
    sound_file_something file;
    int length;

#ifndef DEBUG
    if (!IsDeactivated()) {
#endif

        lastInUseState       = m_inUse;
        m_inUse              = true;
        pSound               = pLoad->m_pSound;
        pSound->m_loadStatus = kLoadStarted;
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
        pSound->m_pSoundData = (table *)vmemory::Unknown(128, NULL);
        set_current_mempool(pPreviousMempool);

        sprintf(pLoad->m_fileName, "%s%s", GetFilePath(5), file.filename);
        DVDOpen(pLoad->m_fileName, &m_fileHandle);
        DriveStatus(0, 0);
        DVDReadAsyncPrio(&m_fileHandle, pSound->m_pSoundData, 128, 0, (DVDCallback)DVDReadCallback, 2);
        m_inUse = lastInUseState;

#ifndef DEBUG
    }
#endif
}

void SoundSystem::AMPushBufferedCallback(char *path) {
    bool lastInUseState;
    sound_load *pLoad;

#ifndef DEBUG
    if (!gSoundSystem.IsDeactivated()) {
#endif

        lastInUseState = gSoundSystem.GetInUse();
        gSoundSystem.SetInUse(true);
        DEBUGASSERTLINE(437, gSoundSystem.GetCurLoadIndex() != kNoCurrentLoad);
        pLoad = gSoundSystem.GetCurAudioLodeQueueSlot();
        DEBUGASSERTLINE(441, strcmp(path, pLoad->m_fileName) == 0);
        pLoad->m_pSound->m_loadStatus = kLoadJustCompleted;
        gSoundSystem.SetInUse(lastInUseState);

#ifndef DEBUG
    }
#endif
}

sound_load *SoundSystem::GetNextFreeAudioLoadQueueEntry() {
    bool lastInUseState;
    u32 currentIndex;
    int index;

    lastInUseState = m_inUse;
    m_inUse        = true;
    currentIndex   = m_CurLoadIndex;
    if (m_CurLoadIndex == 0xFFFFFFFF) {
        currentIndex = 0;
    }
    for (int i = 0; i < MAX_SOUND_LOAD_QUEUE_ENTRIES; i++) {
        index = i + currentIndex;
        if (index / MAX_SOUND_LOAD_QUEUE_ENTRIES != 0) {
            index %= MAX_SOUND_LOAD_QUEUE_ENTRIES;
        }
        DEBUGASSERTLINE(465, index >= 0 && index <= MAX_SOUND_LOAD_QUEUE_ENTRIES);
        if (m_audioLoadQueue[index].m_pSound == NULL) {
            m_inUse = lastInUseState;
            return &m_audioLoadQueue[index];
        }
    }
    m_inUse = lastInUseState;
    return NULL;
}

sound_load *SoundSystem::GetNextAudioLoadQueueEntry() {
    bool lastInUseState = m_inUse;
    u32 currentIndex;
    int index;
    m_inUse = true;
    if (m_LoadQueueEntries == 0) {
        m_CurLoadIndex = 0xFFFFFFFF;
        m_inUse        = lastInUseState;
        return NULL;
    } else {
        currentIndex = m_CurLoadIndex;
        if (m_CurLoadIndex == 0xFFFFFFFF) {
            currentIndex = 0;
        }
        for (int i = 0; i < MAX_SOUND_LOAD_QUEUE_ENTRIES; i++) {
            index = i + currentIndex;
            if (index / MAX_SOUND_LOAD_QUEUE_ENTRIES != 0) {
                index %= MAX_SOUND_LOAD_QUEUE_ENTRIES;
            }
            DEBUGASSERTLINE(501, index >= 0 && index <= MAX_SOUND_LOAD_QUEUE_ENTRIES);
            if (m_audioLoadQueue[index].m_pSound) {
                m_CurLoadIndex = index;
                m_inUse        = lastInUseState;
                return &m_audioLoadQueue[index];
            }
        }
        DEBUGASSERTLINE(511, 0);
        m_inUse = lastInUseState;
        return NULL;
    }
}

void SoundSystem::InitializeSoundTable(sound_load *pLoad) {
    sound_file *pSound;

#ifndef DEBUG
    if (!IsDeactivated()) {
#endif

        bool lastInUseState = m_inUse;
        m_inUse             = true;
        DEBUGASSERTLINE(523, pLoad->m_pSound->m_loadStatus == kLoadJustCompleted);

        pSound               = pLoad->m_pSound;
        pSound->m_loadStatus = kLoadReady;
        SPInitSoundTable(pSound->m_pSoundData, pLoad->m_aramStart, m_AMZeroBuffer);
        ClearSoundLoadQueueEntry(pLoad);
        m_LoadQueueEntries -= 1;
        m_inUse = lastInUseState;

#ifndef DEBUG
    }
#endif
}

void SoundSystem::SetAmbientSoundPanAndVolume(int instance, float pan, float volume) {
    bool lastInUseState;
    bool lastInterruptState;
    ambient_sound *pAmbientSound;

    lastInUseState = m_inUse;
    m_inUse        = true;
    DEBUGASSERTLINE(547, instance >= 0 && instance < MAX_AMBIENT_SOUNDS);

    pAmbientSound      = &m_ambientSounds[instance];
    lastInterruptState = OSDisableInterrupts();

    pAmbientSound->m_AXARTVolume.attenuation = ClampVolume(volume);
    pAmbientSound->m_AXARTPanning.pan        = ConvertPan(pan);

    OSRestoreInterrupts(lastInterruptState);
    m_inUse = lastInUseState;
}

void SoundSystem::PlayAmbientSound(int index) {
    bool lastInUseState;
    ambient_sound *pAmbientSound;
    int i;
    ambient_sound *pCurrentAmbientSoundInSlot;
    SOUND_ENTRY *pSound;
    AXVPB *pVoice;
    AXART_SOUND *pAXARTSound;
    AXART_VOLUME *pAXARTVolume;
    AXART_PANNING *pAXARTPanning;

    if (!field16_0xee36) {
        lastInUseState = m_inUse;
        m_inUse        = true;
        pAmbientSound  = &m_ambientSounds[index];
        if (!pAmbientSound->finished_playing && pAmbientSound->m_pFile->m_pSoundData &&
            pAmbientSound->m_pFile->m_loadStatus == kLoadReady) {
            for (i = 0; i < MAX_AMBIENT_SOUNDS; i++) {
                pCurrentAmbientSoundInSlot = &m_ambientSounds[i];
                if (pCurrentAmbientSoundInSlot->m_pFile) {
                    if (strcmp(pAmbientSound->m_pFile->m_filename, pCurrentAmbientSoundInSlot->m_pFile->m_filename) == 0) {
                        if (pAmbientSound->finished_playing) {
                            m_inUse = lastInUseState;
                            return;
                        }
                    }
                }
            }
            pSound = SPGetSoundEntry(pAmbientSound->m_pFile->m_pSoundData, 0);
            DEBUGASSERTLINE(627, pSound);
            pAmbientSound->m_pVoice = AXAcquireVoice(15, 0, 0);
            pVoice                  = pAmbientSound->m_pVoice;
            DEBUGASSERTLINE(632, pVoice);
            if (!pVoice) {
                m_inUse = lastInUseState;
                return;
            }
            AXSetVoicePriority(pVoice, 5);
            SPPrepareSound(pSound, pVoice, pSound->sampleRate);
            pAXARTSound = &pAmbientSound->m_AXArtSound;
            AXARTInitSound(pAXARTSound, pAmbientSound->m_pVoice, pSound->sampleRate);
            pAXARTVolume = &pAmbientSound->m_AXARTVolume;
            AXARTInitArtVolume(pAXARTVolume);
            pAXARTVolume->attenuation = pAmbientSound->attenuation;
            AXARTAddArticulator(pAXARTSound, (AXART_ART *)pAXARTVolume);
            pAXARTPanning = &pAmbientSound->m_AXARTPanning;
            AXARTInitArtPanning(pAXARTPanning);
            pAXARTPanning->pan  = pAmbientSound->pan;
            pAXARTPanning->span = 127;
            AXARTAddArticulator(pAXARTSound, (AXART_ART *)pAXARTPanning);
            AXARTAddSound(pAXARTSound);
            AXSetVoiceState(pVoice, 1);
            pAmbientSound->finished_playing = true;
        }
        m_inUse = lastInUseState;
    }
}

void SoundSystem::CleanupPlayedAmbientSound(int index) {
    bool lastInUseState          = m_inUse;
    m_inUse                      = true;
    ambient_sound *pAmbientSound = &m_ambientSounds[index];
    if (pAmbientSound->finished_playing) {
        AXARTRemoveSound(&pAmbientSound->m_AXArtSound);
        if (pAmbientSound->m_pVoice && pAmbientSound->m_pVoice->priority) {
            AXFreeVoice(pAmbientSound->m_pVoice);
            pAmbientSound->m_pVoice = NULL;
        }
        pAmbientSound->finished_playing = false;
    }
    m_inUse = lastInUseState;
}

void SoundSystem::PlaySndInstance(snd_instance *pSndInstance, s32 pan, float volume, u32 index, bool unk3, bool unk4) {
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

    m_inUse                   = true;
    sound_file *pFile         = pSndInstance->m_pSound;
    pSndInstance->index       = index;
    pSndInstance->index2      = index;
    pSndInstance->attenuation = ClampVolume(volume);
    pSndInstance->pan         = ConvertPan(pan);
    if (unk4) {
        pSound = SPGetSoundEntry(pSndInstance->m_pSound->m_pSoundData, 0);
        DEBUGASSERTLINE(715, pSound);
        pSndInstance->m_pVoice = AXAcquireVoice(15, 0, 0);
        pVoice                 = pSndInstance->m_pVoice;
        DEBUGASSERTLINE(720, pVoice);
        if (!pVoice) {
            m_inUse = lastInUseState;
            return;
        }
        AXSetVoicePriority(pVoice, 5);
        SPPrepareSound(pSound, pVoice, pSound->sampleRate);
        pAXARTSound = &pSndInstance->m_AXArtSound;
        AXARTInitSound(pAXARTSound, pSndInstance->m_pVoice, pSound->sampleRate);
        pAXARTVolume = &pSndInstance->m_AXARTVolume;
        AXARTInitArtVolume(pAXARTVolume);
        pAXARTVolume->attenuation = pSndInstance->attenuation;
        AXARTAddArticulator(pAXARTSound, (AXART_ART *)pAXARTVolume);
        pAXARTPanning = &pSndInstance->m_AXARTPanning;
        AXARTInitArtPanning(pAXARTPanning);
        pAXARTPanning->pan  = pSndInstance->pan;
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
        pSndInstance->finished_playing = true;
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

    for (int i = 0; i < MAX_SND_INSTANCES; i++) {
    }
    for (int i = 0; i < MAX_AMBIENT_SOUNDS; i++) {
        m_ambientSounds[i].index = -1;
    }

    ARFree(&length);
    DEBUGASSERTLINE(1087, length == AUDIO_BLOCK_SIZE_BYTES);
#ifdef NON_MATCHING
    AXARTQuit(); // Needed this to prevent null reference exception race condition in sound effect processing after reinitialization.
#endif
    InitializeAudio();
    lastInUseState = m_inUse;
}

void SoundSystem::InitializeAudio(void) {
    bool lastInUseState;
    sound_file *file;

    lastInUseState = m_inUse;
    m_inUse        = TRUE;

    Gamemem_info.GetSoundMempool()->ResetOffset();

    AMInit(ARAlloc(0xc00000), 0xc00000);
    m_AMZeroBuffer = AMGetZeroBuffer();
    memset(m_sndInstances, 0, 0x1980);
    memset(m_ambientSounds, 0, 0x1100);

    m_CurLoadIndex     = -1;
    m_LoadQueueEntries = 0;

    for (int i = 0; i < MAX_SOUND_LOAD_QUEUE_ENTRIES; i++) {
        ClearSoundLoadQueueEntry(&m_audioLoadQueue[i]);
    }
    for (int i = 0; i < MAX_PLAY_SLOTS; i++) {
        ClearPlaySlot(&m_PlaySlots[i]);
    }
    for (int i = 0; i < MAX_SOUNDS; i++) {
        file               = &m_audio_file[i];
        file->m_pSoundData = NULL;
        file->m_loadStatus = 0;
        if (i > m_lastPersist) {
            file->m_filename[0] = '\0';
        }
    }
    if (m_lastPersist != 0) {
        m_curEntry = m_lastPersist + 1;
    } else {
        m_curEntry = 0;
    }
    m_inUse = lastInUseState;
}

play_slot *SoundSystem::GetFreePlaySlot(void) {
    bool lastInUseState;

    lastInUseState = m_inUse;
    m_inUse        = true;
    for (int i = 0; i < MAX_PLAY_SLOTS; i++) {
        if (m_PlaySlots[i].m_pInstance != NULL) {
            continue;
        }
        m_inUse = lastInUseState;
        return &m_PlaySlots[i];
    }
    m_inUse = lastInUseState;
    return NULL;
}

void SoundSystem::LoadNewSoundsFromDisk(void) {
    bool lastInUseState = m_inUse;
    m_inUse             = true;
    while (m_LoadQueueEntries) {

        DriveStatus(0, 0);

        CacheSoundFromDisk();
    }
    m_inUse = lastInUseState;
}
