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

#define kLoadNotStarted     0
#define kLoadInitiated      1
#define kLoadStarted        2
#define kLoadRead           3
#define kLoadJustCompleted  4
#define kLoadReady          5

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

class ambient_sound {
  public:
    AXVPB *m_pVoice;
    sound_file *m_pFile;
    AXART_SOUND m_AXArtSound;
    AXART_VOLUME m_AXARTVolume;
    AXART_PANNING m_AXARTPanning;
    int index;
    int index2;
    s32 attenuation;
    s8 pan;
    bool finished_playing;
    bool unk42;
    bool unk43;
};

class snd_instance {
  public:
    AXVPB *m_pVoice;
    sound_file *m_pSound;
    AXART_SOUND m_AXArtSound;
    AXART_VOLUME m_AXARTVolume;
    AXART_PANNING m_AXARTPanning;
    int index;
    int index2;
    s32 attenuation;
    s8 pan;
    bool finished_playing;
    bool unk42;
    bool unk43;
};

class play_slot {
  public:
    snd_instance *m_pInstance;
    s32 m_LoadStatus;
    float m_AXARTVolume;
    int unkC;
    bool unk10;
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
    static void *AllocateReverbMemoryNotImplemented(u32 unk);
    static void FreeReverbMemoryNotImplemented(void *);
    u32 GetEntry(char *filename, bool bIsPersistent, bool bFindExisting);
    void CacheSoundFromDisk(void);
    sound_load *AddToLoadQueue(int index);
    static void DVDReadCallback(void);
    void LoadSoundFile(sound_load *pLoad);
    static void AMPushBufferedCallback(char *path);
    sound_load *GetNextFreeAudioLoadQueueEntry();
    sound_load *GetNextAudioLoadQueueEntry(void);
    void InitializeSoundTable(sound_load *pLoad);
    void SetAmbientSoundPanAndVolume(int instance, float pan, float volume);
    void PlaySndInstance(snd_instance *pSoundEffect, s32 pan, float m_AXARTVolume, u32 index, bool unk3, bool unk4);
    void PlayAmbientSound(int index);
    void CleanupPlayedAmbientSound(int index);
    void ReinitializeAudio(bool state);
    void InitializeAudio(void);
    play_slot *GetFreePlaySlot(void);
    void LoadNewSoundsFromDisk(void);
    snd_instance *GetSndInstance(int index) { return &m_sndInstances[index]; };
    ambient_sound *GetAmbientSound(int index) { return &m_ambientSounds[index]; };
    bool GetUnknown() { return field16_0xee36; };
    bool GetUnknown2() { return field17_0xee37; };
    bool GetUnknown3() { return field14_0xee34; };
#ifndef DEBUG
    bool IsDeactivated() { return m_deactivated; };
#endif
    void SetProcessingQueue(bool processing) { m_processing_queue = processing; };
    bool GetInUse(void) { return m_inUse; };
    void ClearPlaySlot(play_slot *slot) {
        slot->m_pInstance   = NULL;
        slot->m_LoadStatus  = 0;
        slot->m_AXARTVolume = 0.0f;
    };
    char *GetBuffer(void) { return m_buffer; };
    DVDFileInfo *GetDVDFileHandle() { return &m_fileHandle; };
    sound_load *GetCurAudioLodeQueueSlot() {
        if (m_CurLoadIndex == -1) {
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
    /* 0x48 */ char field4_0x48[340];
    /* 0x19c */ s32 m_AMZeroBuffer;
    /* 0x1a0 */ u32 *stack_index_addr[3];
    /* 0x1ac */ u32 m_curEntry;
    /* 0x1b0 */ u32 m_lastPersist;
    /* 0x1b4 */ sound_file m_audio_file[1024];
    /* 0xb1b4 */ snd_instance m_sndInstances[96];
    /* 0xcb34 */ ambient_sound m_ambientSounds[64];
    /* 0xdc34 */ sound_load m_audioLoadQueue[32];
    /* 0xe434 */ play_slot m_PlaySlots[128];
    /* 0xee34 */ bool field14_0xee34;
    /* 0xee35 */ bool m_inUse;
    /* 0xee36 */ bool field16_0xee36;
    /* 0xee37 */ bool field17_0xee37;
    /* 0xee38 */ bool m_processing_queue;
#ifndef DEBUG
    /* 0xee39 */ bool m_deactivated;
#endif
}; // Size: 0xee3a

extern SoundSystem gSoundSystem;

#endif // _H_GAME_SOUNDSYSTEM_