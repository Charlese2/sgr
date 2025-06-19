#include "dolphin/ax.h"
#include "dolphin/dvd.h"
#include "dolphin/axart.h"
#include "dolphin/sp.h"
#include "game/macros.h"
#include "macros.h"
#include "game/NGCSystem.h"

#define AUDIO_BLOCK_SIZE_BYTES       0xc00000
#define MAX_SOUND_LOAD_QUEUE_ENTRIES 32
#define MAX_PLAY_SLOTS               128
#define MAX_AMBIANCE_EFFECT_SLOTS    96
#define MAX_SOUND_EFFECT_SLOTS       64
#define MAX_SOUNDS                   1024
#define MAX_SOUND_FILE_NAME_LENGTH   32
#define MAX_SPT_FILE_SIZE            128

#define kNoCurrentLoad -1

#define kLoadInitiated      1
#define kLoadStarted        2
#define kLoadAMPushBuffered 3
#define kLoadJustCompleted  4
#define kLoadCached         5

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

class sound {
  public:
    table *m_pTable;
    u32 m_LoadStatus;
    int unk8;
    char m_filename[MAX_SOUND_FILE_NAME_LENGTH];
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    bool unk40;
    bool playing;
    bool unk42;
    int unk[64];
    u16 unk144;
    u16 unk146;
};

typedef struct audio_file {
    table *m_pTable;
    u32 m_LoadStatus;
    int *unk8;
    char file_name[MAX_SOUND_FILE_NAME_LENGTH];
} audio_file;

class soundEffect {
  public:
    AXVPB *m_pVoice;
    audio_file *m_pFile;
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

class ambienceEffect {
  public:
    AXVPB *m_pVoice;
    audio_file *m_pFile;
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

class playSlot {
  public:
    ambienceEffect *m_pInstance;
    s32 m_LoadStatus;
    float m_AXARTVolume;
    int unkC;
    bool unk10;
};

class audio_load_cache {
  public:
    sound *m_pSound;
    u32 m_aramStart;
    char m_fileName[56];
};

class sound_file_something {
  public:
    char filename[MAX_SOUND_FILE_NAME_LENGTH];
    int unk;
};

class SoundSystem {
  public:
    SoundSystem();
    static void ProcessSoundsEffects(void);
    static void RemoveAmbienceEffect(ambienceEffect *sound);
    void *GetAudioFileSomethingAtIndex(int index);
    void Initialize(void);
    static void *AllocateReverbMemoryNotImplemented(u32 unk);
    static void FreeReverbMemoryNotImplemented(void *);
    u32 GetEntry(char *filename, bool bIsPersistent, bool bFindExisting);
    void CacheSoundFromDisk(void);
    void LoadUncachedSoundFromDisk(void);
    static void DVDReadCallback(void);
    void LoadAudioFile(audio_load_cache *pLoad);
    static void AMPushBufferedCallback(char *path);
    audio_load_cache *GetNextAudioLoadCacheEntry(void);
    void CacheSound(audio_load_cache *pLoad);
    void PlayAmbienceEffect(ambienceEffect *pSoundEffect, s32 pan, float m_AXARTVolume, u32 index, bool unk3, bool unk4);
    void PlaySoundEffect(int index);
    void CleanupPlayedSound(int index);
    void ReinitializeAudio(bool state);
    void InitializeAudio(void);
    playSlot *GetFreePlaySlot(void);
    void LoadNewSoundsFromDisk(void);
    ambienceEffect *GetAmbianceEffect(int index) { return &m_ambienceEffects[index]; };
    soundEffect *GetSoundEffect(int index) { return &m_soundEffects[index]; };
    bool GetUnknown() { return field16_0xee36; };
    bool GetUnknown2() { return field17_0xee37; };
    bool GetUnknown3() { return field14_0xee34; };
#ifndef DEBUG
    bool IsDeactivated() { return m_deativated; };
#endif
    void SetProcessingQueue(bool processing) { m_processing_queue = processing; };
    bool GetInUse(void) { return m_inUse; };
    void ClearPlaySlot(playSlot *slot) {
        slot->m_pInstance   = NULL;
        slot->m_LoadStatus  = 0;
        slot->m_AXARTVolume = 0.0f;
    };
    char *GetBuffer(void) { return m_buffer; };
    DVDFileInfo *GetDVDFileHandle() { return &m_fileHandle; };
    audio_load_cache *GetCurAudioLodeCacheSlot() {
        if (m_CurLoadIndex == -1) {
            return NULL;
        } else {
            return &m_audioLoadcache[m_CurLoadIndex];
        }
    };
    u32 GetCurLoadIndex() { return m_CurLoadIndex; };
    void SetInUse(bool inUse) { m_inUse = inUse; };
    void InitializeAudioLoadCache(audio_load_cache *cache) {
        cache->m_pSound      = NULL;
        cache->m_aramStart   = 0;
        cache->m_fileName[0] = '\0';
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
    /* 0x40 */ u32 m_CacheQueueRemaining;
    /* 0x44 */ u32 m_CurLoadIndex;
    /* 0x48 */ char field4_0x48[340];
    /* 0x19c */ s32 m_AMZeroBuffer;
    /* 0x1a0 */ u32 *stack_index_addr[3];
    /* 0x1ac */ u32 m_curEntry;
    /* 0x1b0 */ u32 m_lastPersist;
    /* 0x1b4 */ audio_file m_audio_file[1024];
    /* 0xb1b4 */ ambienceEffect m_ambienceEffects[96];
    /* 0xcb34 */ soundEffect m_soundEffects[64];
    /* 0xdc34 */ audio_load_cache m_audioLoadcache[32];
    /* 0xe434 */ playSlot m_PlaySlots[128];
    /* 0xee34 */ bool field14_0xee34;
    /* 0xee35 */ bool m_inUse;
    /* 0xee36 */ bool field16_0xee36;
    /* 0xee37 */ bool field17_0xee37;
    /* 0xee38 */ bool m_processing_queue;
#ifndef DEBUG
    /* 0xee39 */ bool m_deativated;
#endif
}; // Size: 0xee3a

extern SoundSystem gSoundSystem;
