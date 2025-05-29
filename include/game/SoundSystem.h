#include "NMWException.h"
#include "dolphin/ax.h"
#include "dolphin/dvd.h"

extern "C" {
#include "dolphin/axart.h"
#include "dolphin/sp.h"
}

#define AUDIO_BLOCK_SIZE_BYTES       0xc00000
#define MAX_SOUND_LOAD_QUEUE_ENTRIES 32

const bool kLoadInitiated = true;

typedef unsigned char byte;
typedef unsigned char undefined;
typedef unsigned short undefined2;
typedef unsigned int undefined4;

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
    char file_name[20];
    u32 field1_0x14;
    u32 field2_0x18;
    u32 field3_0x1c;
    u32 field4_0x20;
    u32 field5_0x24;
    u32 field6_0x28;
} audio_file;

struct soundEffect {
    AXVPB *axvpb;
    table *table;
    AXART_SOUND sound;
    AXART_VOLUME volume;
    AXART_PANNING panning;
    int field6_0x34;
    undefined4 field7_0x38;
    s32 attenuation;
    u8 pan;
    bool sound_finished_playing;
    undefined field11_0x42;
    undefined field12_0x43;
};

typedef struct audio2 {
    sound *sound;
    struct CommandList *field1_0x4;
    undefined *field2_0x8;
    undefined *field3_0xc;
    undefined *field4_0x10;
    undefined *field5_0x14;
    undefined4 field6_0x18;
    undefined4 field7_0x1c;
    undefined4 field8_0x20;
    undefined *field9_0x24;
    undefined field10_0x28;
    undefined field11_0x29;
    undefined field12_0x2a;
    undefined field13_0x2b;
    undefined2 field14_0x2c;
    undefined field15_0x2e;
    undefined field16_0x2f;
    int field17_0x30;
    int field18_0x34;
    undefined4 field19_0x38;
    byte field20_0x3c;
    bool field21_0x3d;
    bool field22_0x3e;
    undefined field23_0x3f;
    undefined *field24_0x40;
} audio2;

typedef struct new_struct {
    int sound_index;
    int field1_0x4;
    undefined4 field2_0x8;
    float field3_0xc;
    struct soundEffect *field4_0x10;
    struct SomeSFXStruct *field5_0x14;
    struct astruct_7 *field6_0x18;
    undefined4 field7_0x1c;
    undefined *field8_0x20;
    sound_slot *field9_0x24;
    undefined4 field10_0x28;
    undefined4 field11_0x2c;
    undefined4 field12_0x30;
    undefined4 field13_0x34;
    undefined4 field14_0x38;
    undefined4 field15_0x3c;
    undefined4 field16_0x40;
} new_struct;

typedef struct {
    sound *m_pSound;
    int field1_0x4;
    char m_fileName[56];
} audio_load_cache;

struct unk {
    undefined4 field0_0x0;
    undefined4 field1_0x4;
};

class SoundSystem {
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
    /* 0xb1b8 */ audio2 m_SomeAudio[96];
    /* 0xcb34 */ soundEffect m_soundEffect[64];
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

    sound *GetAudio(int index) { return m_SomeAudio[index].sound; };

    soundEffect *GetSoundEffect(int index) { return &m_soundEffect[index]; };
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
