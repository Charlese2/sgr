#include "game/memory.h"
#include "game/effect_mem.h"
#include "game/vector.h"
#include "game/timestamp.h"
#include "dolphin/types.h"

#define MAX_EFFECTS_PER_LEVEL   64
#define MAX_EFFECT_NAME_LEN     32
#define MAX_LARGE_EFFECT_SLOTS  3
#define MAX_SMALL_EFFECT_SLOTS  5
#define MAX_MODAL_RFX_LIST_SIZE 10
#define LARGE_EFFECT_SLOT_SIZE  0x3a000
#define SMALL_EFFECT_SLOT_SIZE  0x1e000

extern int Num_level_effects;

int toggle_effects_show_counters(void);
int toggle_effects_show_slot_usage(void);
int toggle_show_fx_prints(void);

class effect_slot {
  public:
    bool m_inUse;
    u8 *m_slotMemory;
    r_header *m_rheader;
    int m_slot_type;
    bool m_locked;
};

class effect {
  public:
    int unk0;
    int unk4;
    int type;
    vector3x3 unkC;
    vector3 unk30;
    vector3 unk3C;
    int unk48;
    vector3 unk4C;
    vector3 unk58;
    vector3 unk64;
    char unk92;
    GameTimer unkE8;
    vector3 unkC8;

    static void UnlockAllSlots();
    void Initialize();
    static int FreeEffectSlot(effect_slot *slot);
    static void FreeAllEffectSlots();
    static u8 *LoadSpellslotVfx(u32 size, r_header *rheader);
    void Unknown2();
};

class effect_mem_slot_info {
  public:
    u8 *m_memory;
    effect_slot *m_large_slots;
    u32 m_total_large_slots;
    u32 m_large_effect_slot_size;
    u32 m_used_large_slots;
    effect_slot *m_small_slots;
    u32 m_total_small_slots;
    u32 m_small_effect_slot_size;
    u32 m_used_small_slots;
};
