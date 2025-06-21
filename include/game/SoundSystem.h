#ifndef _H_GAME_SOUNDSYSTEM_
#define _H_GAME_SOUNDSYSTEM_

#include "dolphin/ax.h"
#include "dolphin/dvd.h"
#include "dolphin/axart.h"
#include "dolphin/sp.h"
#include "game/macros.h"
#include "macros.h"
#include "game/NGCSystem.h"
#include "game/SoundConvert.h"

#define AUDIO_BLOCK_SIZE_BYTES       0xc00000
#define MAX_SOUND_LOAD_QUEUE_ENTRIES 32
#define MAX_SOUND_FILE_NAME_LENGTH   32
#define MAX_PLAY_SLOTS               128
#define MAX_SOUNDS                   1024
#define MAX_SPT_FILE_SIZE            128

#define kNoCurrentLoad -1

#define kLoadNotStarted    0
#define kLoadInitiated     1
#define kLoadStarted       2
#define kLoadRead          3
#define kLoadJustCompleted 4
#define kLoadReady         5

enum SndInstanceType { SND_INSTANCE = 6, AMBIENT_SOUND };

typedef struct {
    bool field0_0x0;
    bool filed1_0x0;
    bool field2_0x2;
    bool field3_0x3;
    bool field4_0x4;
    bool field5_0x5;
    bool field6_0x6;
    bool field7_0x7;
    float sound_effect_volume;
    float voice_volume;
    float music_volume;
    bool special_sound_mode;
} SoundInfo;

class sound_data {
  public:
    char unk[128];
};

class sound_file {
  public:
    table *m_pSoundData;
    u32 m_loadStatus;
    sound_header *m_pSoundHeader;
    char m_filename[MAX_SOUND_FILE_NAME_LENGTH];
};

class sound_file_something {
  public:
    char filename[MAX_SOUND_FILE_NAME_LENGTH];
    int unk;
};

class snd_instance {
  public:
    AXVPB *m_pVoice;
    sound_file *m_pSound;
    AXART_SOUND m_AXArtSound;
    AXART_VOLUME m_AXARTVolume;
    AXART_PANNING m_AXARTPanning;
    int m_length;
    int m_lengthMilliseconds;
    s32 m_attenuation;
    s8 m_pan;
    bool m_finishedPlaying;
    bool m_AXVoiceState;
    bool unk43;
};

class play_slot {
  public:
    snd_instance *m_pInstance;
    s32 m_pan;
    float m_volume;
    int m_length;
    bool m_bIsSndInstance;
};

class sound_load {
  public:
    sound_file *m_pSound;
    u32 m_aramStart;
    char m_fileName[56];
};

class SoundSystem {
  public:
    SoundSystem();
    static void ProcessSounds(void);
    static void RemoveSndInstance(snd_instance *sound);
    sound_header *GetAudioFileSoundHeader(int index);
    void Initialize(void);
    static void *AllocateReverbMemory(u32 unk);
    static void FreeReverbMemory(void *);
    u32 GetEntry(char *filename, bool bIsPersistent, bool bFindExisting);
    void CacheSoundFromDisk(void);
    sound_load *AddToLoadQueue(int instance);
    static void DVDReadCallback(void);
    void LoadSoundFile(sound_load *pLoad);
    static void AMPushBufferedCallback(char *path);
    sound_load *GetNextFreeAudioLoadQueueEntry();
    sound_load *GetNextAudioLoadQueueEntry(void);
    void InitializeSoundTable(sound_load *pLoad);
    void SetAmbientSoundPanAndVolume(int instance, float pan, float volume);
    void BlankUnknown1(int unk, float unk2);
    void SetSndInstancePanAndVolume(int instance, float pan, float volume);
    int PlayAmbientSoundFile(int instance, s32 pan, float volume, int length);
    void BlankUnknown2(int unk, float unk2, int unk3, float unk4);
    int PlaySndInstanceFile(int instance, s32 pan, float volume);
    void ClearSndInstance(int instance, SndInstanceType type);
    int IsSndInstanceInUse(int instance, SndInstanceType type);
#ifdef DEBUG
    void Shutdown(void);
#else
    void Shutdown(bool);
#endif
    void PlaySndInstance(snd_instance *pSndInstance, s32 pan, float volume, u32 length, bool axVoiceState, bool bIsSndInstance);
    void PlayAmbientSound(int instance);
    void CleanupPlayedAmbientSound(int index);
    void ReinitializeAudio(bool state);
    void InitializeAudio(void);
    play_slot *GetSndInstancePlaySlot(snd_instance *pInstance);
    play_slot *GetFreePlaySlot(sound_file *pSound);
    void LoadNewSoundsFromDisk(void);
    void EnableSndInstanceAXVoiceState();
    void EnableAmbientSoundAXVoiceState();
    void DisableSndInstanceAXVoiceState();
    void DisableAmbientSoundAXVoiceState();
    bool IsSndInstanceAdded(snd_instance *pInstance);
    bool IsAmbientSoundAdded(snd_instance *pInstance);

    snd_instance *GetSndInstance(int index) { return &m_sndInstances[index]; };
    snd_instance *GetAmbientSound(int index) { return &m_ambientSounds[index]; };
    bool GetAmbientSoundAXVoiceState() { return m_AmbientSoundAXVoiceState; };
    bool GetSndInstanceAXVoiceState() { return m_SndInstanceAXVoiceState; };
    bool GetUnknown() { return field14_0xee34; };
#ifndef DEBUG
    bool IsDeactivated() { return m_deactivated; };
#endif
    void SetProcessingQueue(bool processing) { m_processingQueue = processing; };
    bool GetInUse(void) { return m_inUse; };
    void ClearPlaySlot(play_slot *slot) {
        slot->m_pInstance = NULL;
        slot->m_pan       = 0;
        slot->m_volume    = 0.0f;
    };
    char *GetBuffer(void) { return m_buffer; };
    DVDFileInfo *GetDVDFileHandle() { return &m_fileHandle; };
    sound_load *GetCurAudioLoadQueueSlot() {
        if (m_CurLoadIndex == 0xFFFFFFFF) {
            return NULL;
        }
        return &m_audioLoadQueue[m_CurLoadIndex];
    };
    u32 GetCurLoadIndex() { return m_CurLoadIndex; };
    void SetInUse(bool inUse) { m_inUse = inUse; };
    void ClearSoundLoadQueueEntry(sound_load *pLoad) {
        pLoad->m_pSound      = NULL;
        pLoad->m_aramStart   = 0;
        pLoad->m_fileName[0] = '\0';
    };
    u8 ConvertPan(float pan) {
        if (pan < -1.0f || pan > 1.0f) {
            pan = 0.0f;
        }
        pan = 0.5f * (pan + 1.0f);
        pan *= 127.0f;
        return pan;
    };
    u32 ClampVolume(float volume) {
        s32 roundedVolume;
        if (volume > 1.0f) {
            volume = 1.0f;
        } else if (volume < 0.0f) {
            volume = 0.0f;
        }
        roundedVolume = 20.0f * volume;
        DEBUGASSERTLINE(326, roundedVolume >= 0 && roundedVolume <= 20);
        return Volume_table[roundedVolume] << 16;
    };

  private:
    /* 0x00 */ DVDFileInfo m_fileHandle;
    /* 0x3c */ char *m_buffer;
    /* 0x40 */ u32 m_LoadQueueEntries;
    /* 0x44 */ u32 m_CurLoadIndex;
    /* 0x48 */ snd_instance unk[5];
    /* 0x19c */ s32 m_AMZeroBuffer;
    /* 0x1a0 */ u32 *stack_index_addr[3];
    /* 0x1ac */ u32 m_curEntry;
    /* 0x1b0 */ u32 m_lastPersist;
    /* 0x1b4 */ sound_file m_audio_file[1024];
    /* 0xb1b4 */ snd_instance m_sndInstances[96];
    /* 0xcb34 */ snd_instance m_ambientSounds[64];
    /* 0xdc34 */ sound_load m_audioLoadQueue[32];
    /* 0xe434 */ play_slot m_PlaySlots[128];
    /* 0xee34 */ bool field14_0xee34;
    /* 0xee35 */ bool m_inUse;
    /* 0xee36 */ bool m_AmbientSoundAXVoiceState;
    /* 0xee37 */ bool m_SndInstanceAXVoiceState;
    /* 0xee38 */ bool m_processingQueue;
    /* 0xee39 */ bool m_deactivated;
    u8 padding[6];
}; // Size: 0xee40

extern SoundSystem gSoundSystem;

#endif // _H_GAME_SOUNDSYSTEM_
