#include "dolphin/ax.h"
#include "dolphin/dvd.h"

extern "C" {
    #include "dolphin/axart.h"
    #include "dolphin/sp.h"
}

#define AUDIO_BLOCK_SIZE_BYTES       0xc00000
#define MAX_SOUND_LOAD_QUEUE_ENTRIES 32

const bool kLoadInitiated = true;


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

typedef struct {
    AXVPB *next;
    AXVPB *prev;
    float unk8;
    float unkC;
    float unk10;
} sound_slot;

typedef struct {
    AXVPB *axvpb;
    BOOL m_loadStatus;
    AXART_SOUND axart_sound;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    bool unk40;
    bool playing;
    bool unk42;
} sound;

typedef struct audio_file {
    int *unk0;
    int unk4;
    int unk8;
    char file_name[32];
} audio_file;

struct soundEffect {
    AXVPB *axvpb;
    table *table;
    AXART_SOUND sound;
    AXART_VOLUME volume;
    AXART_PANNING panning;
    int index;
    int field7_0x38;
    s32 attenuation;
    u8 pan;
    bool finished_playing;
    bool unk42;
    bool field12_0x43;
};

typedef struct audio2 {
    sound *sound;
    struct CommandList *field1_0x4;
    char *field2_0x8;
    char *field3_0xc;
    char *field4_0x10;
    char *field5_0x14;
    int field6_0x18;
    int field7_0x1c;
    int field8_0x20;
    char *field9_0x24;
    char field10_0x28;
    char field11_0x29;
    char field12_0x2a;
    char field13_0x2b;
    short field14_0x2c;
    char field15_0x2e;
    char field16_0x2f;
    int field17_0x30;
    int index;
    int field19_0x38;
    char field20_0x3c;
    bool field21_0x3d;
    bool field22_0x3e;
    char field23_0x3f;
    char *field24_0x40;
} audio2;

typedef struct new_struct {
    int sound_index;
    int field1_0x4;
    int field2_0x8;
    float field3_0xc;
    struct soundEffect *field4_0x10;
    struct SomeSFXStruct *field5_0x14;
    struct astruct_7 *field6_0x18;
    int field7_0x1c;
    char *field8_0x20;
    sound_slot *field9_0x24;
    int field10_0x28;
    int field11_0x2c;
    int field12_0x30;
    int field13_0x34;
    int field14_0x38;
    int field15_0x3c;
    int field16_0x40;
} new_struct;

typedef struct {
    sound *m_pSound;
    int field1_0x4;
    char m_fileName[56];
} audio_load_cache;

class SoundSystem {
  public:
    SoundSystem();
    void InitializeGlobal(void);
    static void ProcessSoundsEffects(void);
    static void RemoveSound(sound *sound);
    void Initialize(void);
    static void *AllocateReverbMemoryNotImplemented(u32 unk);
    static void FreeReverbMemoryNotImplemented(void *);
    void LoadSoundFromDisk(void);
    void LoadUncachedSoundFromDisk(void);
    audio_load_cache *GetNextAudioLoadCacheEntry(void);
    void AddSound(int index);
    void CopySoundFileToCache();
    void PlaySoundEffect(int index);
    void CleanupPlayedSound(int index);
    void ReinitializeAudio(bool state);
    void InitializeAudio(void);
    sound_slot *GetFreeSoundSlot(void);
    void LoadNewSoundsFromDisk(void);

    sound *GetSoundEffectSound(int index) { return m_ambienceEffects[index].sound; };

    soundEffect *GetSoundEffect(int index) { return &m_soundEffects[index]; };
    bool GetUnknown() { return field22_0xee36; };
    bool GetUnknown2() { return field23_0xee37; };
    void SetProcessingQueue(bool processing) { m_processing_queue = processing; }
    bool InUse(void) { return m_inUse; }
    void InitializeSoundSlot(sound_slot *slot) {
        slot->next = 0;
        slot->prev = 0;
        slot->unk8 = 0.0f;
    }
    char *GetBuffer(void) { return m_buffer; }
    void InitializeAudioLoadCache(audio_load_cache *cache) {
        cache->m_pSound      = NULL;
        cache->field1_0x4    = 0;
        cache->m_fileName[0] = (char)0;
    }

  private:
    /* 0x00 */ DVDFileInfo m_fileHandle;
    /* 0x3c */ char *m_buffer;
    /* 0x40 */ u32 m_number_in_queue_of_sounds_not_preloaded;
    /* 0x44 */ s32 m_next_index;
    /* 0x48 */ char field4_0x48[340];
    /* 0x19c */ s32 m_AMZeroBuffer;
    /* 0x1a0 */ u32 *stack_index_addr[3];
    /* 0x1ac */ s32 m_curEntry;
    /* 0x1b0 */ u32 m_lastPersist;
    /* 0x1b4 */ audio_file m_audio_file[1024];
    /* 0xb1b8 */ audio2 m_ambienceEffects[96];
    /* 0xcb34 */ soundEffect m_soundEffects[64];
    /* 0xdc34 */ audio_load_cache m_audioLoadcache[32];
    /* 0xe434 */ sound_slot m_sound_slots[128];
    /* 0xee34 */ bool field20_0xee34;
    /* 0xee35 */ bool m_inUse;
    /* 0xee36 */ bool field22_0xee36;
    /* 0xee37 */ bool field23_0xee37;
    /* 0xee38 */ bool m_processing_queue;
#ifndef DEBUG
    /* 0xee39 */ bool m_deativated;
#endif
}; // Size: 0xee3a

extern SoundSystem gSoundSystem;

inline int ClampVolume(double volume) {
    int roundedVolume;
    if (volume <= 1.0f) {
        if (volume < 0.0f) {
            volume = 0.0f;
        }
    } else {
        volume = 1.0f;
    }
    roundedVolume = 20.0f * volume;
    return roundedVolume;
}
