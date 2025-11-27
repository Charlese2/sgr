#include "game/SoundSystem.h"
#include "game/frametime.h"
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
extern sound_info soundInfo;
char buffer[2048];

SoundSystem::SoundSystem() {
    m_buffer       = buffer;
    m_AMZeroBuffer = 0;
    m_curEntry     = 0;
    m_initialized  = false;
    m_inUse        = false;
    m_lastPersist  = 0;
#ifndef DEBUG
    m_deactivated = FALSE;
#endif
}

void SoundSystem::ProcessSounds(void) {
    u32 i;
    snd_instance *pSndInstance;
    snd_instance *pAmbientSound;

#ifndef DEBUG
    if (!gSoundSystem.IsDeactivated()) {
#endif
        MIXUpdateSettings();
        AXARTServiceSounds();
        if (!gSoundSystem.GetInUse()) {
            gSoundSystem.SetProcessingSounds(true);
            for (i = 0; i < MAX_SND_INSTANCES; i++) {
                pSndInstance = gSoundSystem.GetSndInstance(i);
                if (!gSoundSystem.GetSndInstanceAXVoiceState() || pSndInstance->m_AXVoiceState) {
                    AXVPB *pVoice = pSndInstance->m_pVoice;
                    if (pVoice) {
                        if (pVoice->pb.state == 0) {
#ifndef DEBUG
                            if (!gSoundSystem.IsDeactivated()) {
#endif
                                gSoundSystem.RemoveSndInstance(pSndInstance);
#ifndef DEBUG
                            }
#endif
                        }
                    }
                }
            }
            if (!gSoundSystem.GetAmbientSoundAXVoiceState()) {
                for (i = 0; i < MAX_AMBIENT_SOUNDS; i++) {
                    pAmbientSound = gSoundSystem.GetAmbientSound(i);
                    AXVPB *pVoice = pAmbientSound->m_pVoice;
                    if (pVoice) {
                        if (pVoice->pb.state == 0) {
                            gSoundSystem.CleanupPlayedAmbientSound(i);
                            if (pAmbientSound->m_length != -1) {
                                pAmbientSound->m_lengthMilliseconds -= reported_frametime * 1000;
                                if (pAmbientSound->m_lengthMilliseconds <= 0) {
                                    gSoundSystem.PlayAmbientSound(i);
                                    pAmbientSound->m_lengthMilliseconds = pAmbientSound->m_length;
                                }
                            }
                        }
                    }
                }
            }
            gSoundSystem.SetProcessingSounds(false);
        }
#ifndef DEBUG
    }
#endif
}

void SoundSystem::RemoveSndInstance(snd_instance *sound) {
    AXARTRemoveSound(&sound->m_AXArtSound);
    sound->m_pSound = NULL;
    if (sound->m_pVoice->priority) {
        AXFreeVoice(sound->m_pVoice);
    }
    sound->m_pVoice          = NULL;
    sound->m_finishedPlaying = false;
}

sound_header *SoundSystem::GetAudioFileSoundHeader(int index) { return m_audio_file[index].m_pSoundHeader; }

void SoundSystem::Initialize() {
    bool lastInUseState;

    lastInUseState = m_inUse;
    m_inUse        = true;
#ifndef DEBUG
    m_deactivated = false;
#endif
    m_AmbientSoundAXVoiceState = false;
    m_SndInstanceAXVoiceState  = false;
    m_initialized              = true;
    ARInit((u32 *)stack_index_addr, 3);
    ARQInit();
    AIInit(0);
    AXInit();
    DTKInit();
    MIXInit();
    AXARTInit();
#ifndef DEBUG
    if (OSGetSoundMode() == 1) {
        soundInfo.m_special_sound_mode = true;
        AXSetMode(0);
        MIXSetSoundMode(1);
    } else {
        soundInfo.m_special_sound_mode = false;
        AXSetMode(0);
        MIXSetSoundMode(0);
    }
#else
    AXSetMode(0);
    MIXSetSoundMode(1);
#endif
    InitializeAudio();
    AXRegisterCallback(ProcessSounds);
    AXFXSetHooks(AllocateReverbMemory, FreeReverbMemory);
    AXSetCompressor(1);
    m_inUse = lastInUseState;
}

void SoundSystem::FreeReverbMemory(void *address) {
#ifdef DEBUG
    DebugError("SoundSystem.cpp", 180, "Reverb memory freeing not implemented.  Get Geoff!");
#else
    DebugError("SoundSystem.cpp", 199, "Reverb memory freeing not implemented.  Get Geoff!");
#endif
}

void *SoundSystem::AllocateReverbMemory(u32 unk) {
#ifdef DEBUG
    DebugError("SoundSystem.cpp", 187, "Reverb memory allocation not implemented.  Get Geoff!");
#else
    DebugError("SoundSystem.cpp", 206, "Reverb memory allocation not implemented.  Get Geoff!");
#endif
    return NULL;
}

u32 SoundSystem::GetEntry(char *filename, bool bIsPersistent, bool bFindExisting) {
    bool lastInUseState;
    u32 i;
    sound_file *pFile;

#ifndef DEBUG
    NGCSystem::DriveStatus(0, 0);
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

                    PlaySndInstance(pPlay->m_pInstance, pPlay->m_pan, pPlay->m_volume, pPlay->m_length, 0, pPlay->m_bIsSndInstance);
                    ClearPlaySlot(pPlay);
                }
            }
        }

        m_inUse = lastInUseState;

#ifndef DEBUG
    }
#endif
}

sound_load *SoundSystem::AddToLoadQueue(int instance) {
    bool lastInUseState;
    sound_file *pSound;
    sound_load *pLoad;

    lastInUseState         = m_inUse;
    m_inUse                = true;
    pSound                 = &m_audio_file[instance];
    pSound->m_pSoundHeader = SoundConvert::GetSoundHeader(pSound->m_filename);
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

        pLoad                = gSoundSystem.GetCurAudioLoadQueueSlot();
        pSound               = pLoad->m_pSound;
        sptLength            = pSound->m_pSoundData->entries;
        pSound->m_pSoundData = (table *)pSound->m_pSoundData->sound; // TODO: Fix type of m_pSoundData
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
    snd_instance *pInstance;
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

        sprintf(pLoad->m_fileName, "%s%s", FileSystem::GetFilePath(5), file.filename);

#ifdef DEBUG
        DVDOpen(pLoad->m_fileName, &m_fileHandle);
        NGCSystem::DriveStatus(0, 0);
        DVDReadAsyncPrio(&m_fileHandle, pSound->m_pSoundData, 128, 0, (DVDCallback)DVDReadCallback, 2);
#else
    if (DVDOpen(pLoad->m_fileName, &m_fileHandle)) {
        NGCSystem::DriveStatus(0, 0);
        DVDReadAsyncPrio(&m_fileHandle, pSound->m_pSoundData, 128, 0, (DVDCallback)DVDReadCallback, 2);
    } else {
        printf("********************************************************\n");
        printf("MISSING SOUND FILE!!!!!!!!!!!!\n");
        printf("File: %s\n", pLoad->m_fileName);
        printf("********************************************************\n");
        m_CurLoadIndex       = 0xFFFFFFFF;
        pSound->m_loadStatus = kLoadNotStarted;
        pLoad->m_pSound      = NULL;
        pLoad->m_aramStart   = 0;
        pLoad->m_fileName[0] = '\0';
        m_LoadQueueEntries--;
        for (int i = 0; i < MAX_PLAY_SLOTS; i++) {
            pInstance = m_PlaySlots[i].m_pInstance;
            if (pInstance && pInstance->m_pSound == pSound) {
                m_PlaySlots[i].m_pInstance = NULL;
                m_PlaySlots[i].m_pan       = 0;
                m_PlaySlots[i].m_volume    = 0.0f;
            }
        }
    }
#endif
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
        pLoad = gSoundSystem.GetCurAudioLoadQueueSlot();
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
    bool lastInUseState;

    lastInUseState = m_inUse;
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
    bool lastInUseState;

#ifndef DEBUG
    if (!IsDeactivated()) {
#endif

        lastInUseState = m_inUse;
        m_inUse        = true;
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
    snd_instance *pAmbientSound;

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

void SoundSystem::BlankUnknown1(int unk, float unk2) {}

void SoundSystem::SetSndInstancePanAndVolume(int instance, float pan, float volume) {
    bool lastInUseState;
    bool lastInterruptState;
    snd_instance *pSndInstance;

    lastInUseState = m_inUse;
    m_inUse        = true;
    DEBUGASSERTLINE(581, instance >= 0 && instance < MAX_SND_INSTANCES);

    pSndInstance       = &m_sndInstances[instance];
    lastInterruptState = OSDisableInterrupts();

    pSndInstance->m_AXARTVolume.attenuation = ClampVolume(volume);
    pSndInstance->m_AXARTPanning.pan        = ConvertPan(pan);

    OSRestoreInterrupts(lastInterruptState);
    m_inUse = lastInUseState;
}

void SoundSystem::PlayAmbientSound(int index) {
    bool lastInUseState;
    snd_instance *pAmbientSound;
    int i;
    snd_instance *pCurrentAmbientSoundInSlot;
    SOUND_ENTRY *pSound;
    AXVPB *pVoice;
    AXART_SOUND *pAXARTSound;
    AXART_VOLUME *pAXARTVolume;
    AXART_PANNING *pAXARTPanning;

    if (!m_AmbientSoundAXVoiceState) {
        lastInUseState = m_inUse;
        m_inUse        = true;
        pAmbientSound  = &m_ambientSounds[index];
        if (!pAmbientSound->m_finishedPlaying && pAmbientSound->m_pSound->m_pSoundData &&
            pAmbientSound->m_pSound->m_loadStatus == kLoadReady) {
            for (i = 0; i < MAX_AMBIENT_SOUNDS; i++) {
                pCurrentAmbientSoundInSlot = &m_ambientSounds[i];
                if (pCurrentAmbientSoundInSlot->m_pSound) {
                    if (strcmp(pAmbientSound->m_pSound->m_filename, pCurrentAmbientSoundInSlot->m_pSound->m_filename) == 0) {
                        if (pAmbientSound->m_finishedPlaying) {
                            m_inUse = lastInUseState;
                            return;
                        }
                    }
                }
            }
            pSound = SPGetSoundEntry(pAmbientSound->m_pSound->m_pSoundData, 0);
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
            pAXARTVolume->attenuation = pAmbientSound->m_attenuation;
            AXARTAddArticulator(pAXARTSound, (AXART_ART *)pAXARTVolume);
            pAXARTPanning = &pAmbientSound->m_AXARTPanning;
            AXARTInitArtPanning(pAXARTPanning);
            pAXARTPanning->pan  = pAmbientSound->m_pan;
            pAXARTPanning->span = 127;
            AXARTAddArticulator(pAXARTSound, (AXART_ART *)pAXARTPanning);
            AXARTAddSound(pAXARTSound);
            AXSetVoiceState(pVoice, 1);
            pAmbientSound->m_finishedPlaying = true;
        }
        m_inUse = lastInUseState;
    }
}

void SoundSystem::CleanupPlayedAmbientSound(int index) {
    bool lastInUseState         = m_inUse;
    m_inUse                     = true;
    snd_instance *pAmbientSound = &m_ambientSounds[index];
    if (pAmbientSound->m_finishedPlaying) {
        AXARTRemoveSound(&pAmbientSound->m_AXArtSound);
        if (pAmbientSound->m_pVoice && pAmbientSound->m_pVoice->priority) {
            AXFreeVoice(pAmbientSound->m_pVoice);
            pAmbientSound->m_pVoice = NULL;
        }
        pAmbientSound->m_finishedPlaying = false;
    }
    m_inUse = lastInUseState;
}

void SoundSystem::PlaySndInstance(snd_instance *pSndInstance, s32 pan, float volume, u32 length, bool axVoiceState, bool bIsSndInstance) {
    bool lastInUseState;
    SOUND_ENTRY *pSound;
    AXVPB *pVoice;
    AXART_SOUND *pAXARTSound;
    AXART_VOLUME *pAXARTVolume;
    AXART_PANNING *pAXARTPanning;

    lastInUseState                     = m_inUse;
    m_inUse                            = true;
    sound_file *pFile                  = pSndInstance->m_pSound;
    pSndInstance->m_length             = length;
    pSndInstance->m_lengthMilliseconds = length;
    pSndInstance->m_attenuation        = ClampVolume(volume);
    pSndInstance->m_pan                = ConvertPan(pan);
    if (bIsSndInstance) {
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
        pAXARTVolume->attenuation = pSndInstance->m_attenuation;
        AXARTAddArticulator(pAXARTSound, (AXART_ART *)pAXARTVolume);
        pAXARTPanning = &pSndInstance->m_AXARTPanning;
        AXARTInitArtPanning(pAXARTPanning);
        pAXARTPanning->pan  = pSndInstance->m_pan;
        pAXARTPanning->span = 127;
        AXARTAddArticulator(pAXARTSound, (AXART_ART *)pAXARTPanning);
        AXARTAddSound(pAXARTSound);
        if (axVoiceState) {
            AXSetVoiceState(pVoice, 0);
        } else {
            AXSetVoiceState(pVoice, 1);
        }
        pSndInstance->m_finishedPlaying = true;
    }
    m_inUse = lastInUseState;
}

int SoundSystem::PlayAmbientSoundFile(int instance, s32 pan, float volume, int length) {
    bool lastInUseState;
    int v;
    snd_instance *pAmbientSound;
    play_slot *pPlay;
    sound_load *pLoad;

    lastInUseState = m_inUse;
    m_inUse        = true;
    for (v = 0; v < MAX_AMBIENT_SOUNDS; v++) {
        if (m_ambientSounds[v].m_pSound == NULL) {
            break;
        }
    }
    ASSERTLINE(779, v < MAX_AMBIENT_SOUNDS);
    pAmbientSound                 = &m_ambientSounds[v];
    pLoad                         = AddToLoadQueue(instance);
    pAmbientSound->m_pSound       = &m_audio_file[instance];
    pAmbientSound->m_AXVoiceState = m_AmbientSoundAXVoiceState;
    if (pLoad == NULL) {
        PlaySndInstance(pAmbientSound, pan, volume, length, m_AmbientSoundAXVoiceState, false);
    } else {
        pPlay = GetFreePlaySlot(pAmbientSound->m_pSound);
        DEBUGASSERTLINE(792, pPlay);
        pPlay->m_pInstance      = pAmbientSound;
        pPlay->m_pan            = pan;
        pPlay->m_volume         = volume;
        pPlay->m_length         = length;
        pPlay->m_bIsSndInstance = false;
    }
    m_inUse = lastInUseState;
    return v;
}

void SoundSystem::BlankUnknown2(int unk, float unk2, int unk3, float unk4) {}

int SoundSystem::PlaySndInstanceFile(int instance, s32 pan, float volume) {
    bool lastInUseState;
    int v;
    snd_instance *pSndInstance;
    play_slot *pPlay;
    sound_load *pLoad;

    lastInUseState = m_inUse;
    m_inUse        = true;
    for (v = 0; v < MAX_SND_INSTANCES; v++) {
        if (m_sndInstances[v].m_pSound == NULL) {
            break;
        }
    }
    ASSERTLINE(918, v < MAX_SND_INSTANCES);
    pSndInstance                 = &m_sndInstances[v];
    pLoad                        = AddToLoadQueue(instance);
    pSndInstance->m_pSound       = &m_audio_file[instance];
    pSndInstance->m_AXVoiceState = m_SndInstanceAXVoiceState;
    if (pLoad == NULL) {
        PlaySndInstance(pSndInstance, pan, volume, -1, 0, true);
    } else {
        pPlay = GetFreePlaySlot(pSndInstance->m_pSound);
        DEBUGASSERTLINE(932, pPlay);
        pPlay->m_pInstance      = pSndInstance;
        pPlay->m_pan            = pan;
        pPlay->m_volume         = volume;
        pPlay->m_length         = -1;
        pPlay->m_bIsSndInstance = true;
    }
    m_inUse = lastInUseState;
    return v;
}

void SoundSystem::ClearSndInstance(int instance, SndInstanceType type) {
    bool lastInUseState;
    snd_instance *pInstance;
    play_slot *pPlay;

    lastInUseState = m_inUse;
    m_inUse        = true;
    if (type == SND_INSTANCE) {
        pInstance = &m_sndInstances[instance];
    } else {
        pInstance = &m_ambientSounds[instance];
    }
    DEBUGASSERTLINE(962, pInstance);
    if (pInstance->m_pVoice == NULL) {
        pPlay = GetSndInstancePlaySlot(pInstance);
        if (pPlay) {
            ClearPlaySlot(pPlay);
        }
    } else {
        AXSetVoiceState(pInstance->m_pVoice, 0);
        pInstance->m_length             = -1;
        pInstance->m_lengthMilliseconds = -1;
    }
    m_inUse = lastInUseState;
}

int SoundSystem::IsSndInstanceInUse(int instance, SndInstanceType type) {
    bool lastInUseState;
    snd_instance *pInstance;
    AXVPB *pVoice;
    play_slot *pPlay;
    bool result;

    lastInUseState = m_inUse;
    m_inUse        = true;
    if (type == SND_INSTANCE) {
        pInstance = &m_sndInstances[instance];
    } else {
        pInstance = &m_ambientSounds[instance];
    }
    pVoice = pInstance->m_pVoice;
    result = false;
    if (pVoice != NULL) {
        if (pVoice->pb.state == 1) {
            result = true;
        }
    } else {
        pPlay = GetSndInstancePlaySlot(pInstance);
        if (pPlay) {
            result = true;
        }
    }
    m_inUse = lastInUseState;
    return result;
}

#ifdef DEBUG
void SoundSystem::Shutdown(void) {
#else
void SoundSystem::Shutdown(bool bShutdownDiskTrack) {
#endif
    bool lastInUseState;

#ifndef DEBUG
    m_deactivated = true;
#endif
    while (m_processingQueue) {
    };
    AXRegisterCallback(NULL);
    ReinitializeAudio(false);
    lastInUseState = m_inUse;
    m_inUse        = true;
#ifdef DEBUG
    if (DVDCheckDisk()) {
#else
    if (bShutdownDiskTrack && DVDCheckDisk()) {
#endif
        DTKShutdown();
    }
    m_inUse = lastInUseState;
}

void SoundSystem::ReinitializeAudio(bool loadNewSounds) {
    u32 length;
    bool lastInUseState;
    int i;

    lastInUseState = m_inUse;
    m_inUse        = true;

    while (m_processingQueue) {
    }
    if (loadNewSounds) {
        LoadNewSoundsFromDisk();
    }

    m_AmbientSoundAXVoiceState = false;
    m_SndInstanceAXVoiceState  = false;

    for (i = 0; i < MAX_SND_INSTANCES; i++) {
        if (IsSndInstanceInUse(i, SND_INSTANCE)) {
            ClearSndInstance(i, SND_INSTANCE);
        }
    }
    for (i = 0; i < MAX_AMBIENT_SOUNDS; i++) {
        m_ambientSounds[i].m_length = -1;
        if (IsSndInstanceInUse(i, AMBIENT_SOUND)) {
            ClearSndInstance(i, AMBIENT_SOUND);
        }
    }

    ARFree(&length);
    DEBUGASSERTLINE(1087, length == AUDIO_BLOCK_SIZE_BYTES);
#ifdef NON_MATCHING
    AXARTQuit(); // Needed this to prevent null reference exception race condition in sound processing after reinitialization.
#endif
    InitializeAudio();
    m_inUse = lastInUseState;
}

void SoundSystem::InitializeAudio(void) {
    bool lastInUseState;
    sound_file *file;

    lastInUseState = m_inUse;
    m_inUse        = true;

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

play_slot *SoundSystem::GetSndInstancePlaySlot(snd_instance *pInstance) {
    bool lastInUseState;

    lastInUseState = m_inUse;
    m_inUse        = true;

    for (int i = 0; i < MAX_PLAY_SLOTS; i++) {
        if (m_PlaySlots[i].m_pInstance != pInstance) {
            continue;
        }
        m_inUse = lastInUseState;
        return &m_PlaySlots[i];
    }
    m_inUse = lastInUseState;
    return NULL;
}

play_slot *SoundSystem::GetFreePlaySlot(sound_file *pSound) {
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
    bool lastInUseState;

    lastInUseState = m_inUse;
    m_inUse        = true;
    while (m_LoadQueueEntries) {
        NGCSystem::DriveStatus(0, 0);
        CacheSoundFromDisk();
    }
    m_inUse = lastInUseState;
}

void SoundSystem::EnableSndInstanceAXVoiceState() {
    bool lastInUseState;
    AXVPB *pVoice;
    int i;

    lastInUseState            = m_inUse;
    m_inUse                   = true;
    m_SndInstanceAXVoiceState = true;
    for (i = 0; i < MAX_SND_INSTANCES; i++) {
        pVoice = m_sndInstances[i].m_pVoice;
        if (pVoice && pVoice->pb.state == 1) {
            AXSetVoiceState(pVoice, 0);
        }
    }
    for (i = 0; i < MAX_PLAY_SLOTS; i++) {
        if (m_PlaySlots[i].m_pInstance) {
            if (IsSndInstanceAdded(m_PlaySlots[i].m_pInstance)) {
                ClearPlaySlot(&m_PlaySlots[i]);
            }
        }
    }
    m_inUse = lastInUseState;
}

void SoundSystem::EnableAmbientSoundAXVoiceState() {
    bool lastInUseState;
    AXVPB *pVoice;
    int i;

    lastInUseState             = m_inUse;
    m_inUse                    = true;
    m_AmbientSoundAXVoiceState = true;
    for (i = 0; i < MAX_AMBIENT_SOUNDS; i++) {
        pVoice = m_ambientSounds[i].m_pVoice;
        if (pVoice && pVoice->pb.state == 1) {
            AXSetVoiceState(pVoice, 0);
        }
    }
    for (i = 0; i < MAX_PLAY_SLOTS; i++) {
        if (m_PlaySlots[i].m_pInstance) {
            if (IsAmbientSoundAdded(m_PlaySlots[i].m_pInstance)) {
                m_PlaySlots[i].m_bIsSndInstance = false;
            }
        }
    }
    m_inUse = lastInUseState;
}

void SoundSystem::DisableSndInstanceAXVoiceState() {
    bool lastInUseState;

    lastInUseState             = m_inUse;
    m_inUse                    = true;
    m_AmbientSoundAXVoiceState = false;
    m_SndInstanceAXVoiceState  = false;
    m_inUse                    = lastInUseState;
}

void SoundSystem::DisableAmbientSoundAXVoiceState() {
    bool lastInUseState;

    lastInUseState             = m_inUse;
    m_inUse                    = true;
    m_AmbientSoundAXVoiceState = false;
    m_SndInstanceAXVoiceState  = false;
    m_inUse                    = lastInUseState;
}

bool SoundSystem::IsSndInstanceAdded(snd_instance *pInstance) {
    bool lastInUseState;
    int i;

    lastInUseState = m_inUse;
    m_inUse        = true;
    for (i = 0; i < MAX_SND_INSTANCES; i++) {
        if (pInstance != &m_sndInstances[i]) {
            continue;
        }
        m_inUse = lastInUseState;
        return true;
    }
    m_inUse = lastInUseState;
    return false;
}

bool SoundSystem::IsAmbientSoundAdded(snd_instance *pInstance) {
    bool lastInUseState;
    int i;

    lastInUseState = m_inUse;
    // @BUG Missing `m_inUse = true`;
    for (i = 0; i < MAX_AMBIENT_SOUNDS; i++) {
        if (pInstance != &m_ambientSounds[i]) {
            continue;
        }
        m_inUse = lastInUseState;
        return true;
    }
    m_inUse = lastInUseState;
    return false;
}
