#include "game/effects.h"
#include "game/vmemory.h"
#include "game/gamemem.h"
#include "game/console.h"
#include "game/macros.h"
#include "game/gr_ngc.h"
#include "game/PackFile.h"
#include "game/PackFileSystem.h"
#include "game/File.h"
#include <stdio.h>

#ifdef DEBUG
console_command Toggle_effects_show_counters("effects_show_counters", "Toggles Effects_show_counters", TOGGLE,
                                             (CommandCallbackInt)toggle_effects_show_counters);
console_command Toggle_effects_show_slot_usage("effects_show_slot_usage", "Toggles Effects_show_slot_usage", TOGGLE,
                                               (CommandCallbackInt)toggle_effects_show_slot_usage);
console_command Toggle_show_fx_prints("show_fx_prints", "Toggles Show_fx_prints", TOGGLE, (CommandCallbackInt)toggle_show_fx_prints);
#endif

int max_rfx = 10;

bool Effects_show_counters;
bool Effects_show_slot_usage;
bool Show_fx_prints;

int Num_level_effects;

effect gEffects[100];
effect_mem_slot_info *Effect_mem_slot_info;

int toggle_effects_show_counters() {
    if (calling_a_command_function) {
        process_command(193);
        if (arg_type & ARGUMENT_TYPE_UNKNOWN7) {
            Effects_show_counters = true;
        } else if (arg_type & ARGUMENT_TYPE_UNKNOWN8) {
            Effects_show_counters = false;
        } else if (arg_type & ARGUMENT_TYPE_UNKNOWN1) {
            Effects_show_counters ^= true;
        }
    }
    if (doing_help_for_comand) {
        sprintf(string_buffer, "Usage: %s [bool]\nSets %s to true or false. If nothing passed, then toggles it\n", "effects_show_counters",
                "Effects_show_counters");
        print_to_console(string_buffer, 0);
    }
    if (checking_status_for_command) {
        sprintf(string_buffer, "%s is %s\n", "effects_show_counters", Effects_show_counters ? "TRUE" : "FALSE");
        print_to_console(string_buffer, 0);
    }
    return Effects_show_counters;
}

int toggle_effects_show_slot_usage() {
    if (calling_a_command_function) {
        process_command(193);
        if (arg_type & ARGUMENT_TYPE_UNKNOWN7) {
            Effects_show_slot_usage = true;
        } else if (arg_type & ARGUMENT_TYPE_UNKNOWN8) {
            Effects_show_slot_usage = false;
        } else if (arg_type & ARGUMENT_TYPE_UNKNOWN1) {
            Effects_show_slot_usage ^= true;
        }
    }
    if (doing_help_for_comand) {
        sprintf(string_buffer, "Usage: %s [bool]\nSets %s to true or false. If nothing passed, then toggles it\n",
                "effects_show_slot_usage", "Effects_show_slot_usage");
        print_to_console(string_buffer, 0);
    }
    if (checking_status_for_command) {
        sprintf(string_buffer, "%s is %s\n", "effects_show_slot_usage", Effects_show_slot_usage ? "TRUE" : "FALSE");
        print_to_console(string_buffer, 0);
    }
    return Effects_show_slot_usage;
}

int toggle_show_fx_prints() {
    if (calling_a_command_function) {
        process_command(193);
        if (arg_type & ARGUMENT_TYPE_UNKNOWN7) {
            Show_fx_prints = true;
        } else if (arg_type & ARGUMENT_TYPE_UNKNOWN8) {
            Show_fx_prints = false;
        } else if (arg_type & ARGUMENT_TYPE_UNKNOWN1) {
            Show_fx_prints ^= true;
        }
    }
    if (doing_help_for_comand) {
        sprintf(string_buffer, "Usage: %s [bool]\nSets %s to true or false. If nothing passed, then toggles it\n", "show_fx_prints",
                "Show_fx_prints");
        print_to_console(string_buffer, 0);
    }
    if (checking_status_for_command) {
        sprintf(string_buffer, "%s is %s\n", "show_fx_prints", Show_fx_prints ? "TRUE" : "FALSE");
        print_to_console(string_buffer, 0);
    }
    return Show_fx_prints;
}

void effect::UnlockAllSlots() {
    u32 i;
    for (i = 0; i < MAX_LARGE_EFFECT_SLOTS; i++) {
        Effect_mem_slot_info->m_large_slots[i].m_locked = false;
    }
    for (i = 0; i < MAX_SMALL_EFFECT_SLOTS; i++) {
        Effect_mem_slot_info->m_small_slots[i].m_locked = false;
    }
}

void effect::Initialize() {
    Mempool *cur_mempool;
    u32 slot_cnt;

    cur_mempool = get_current_mempool();
    set_current_mempool(&Gamemem_info.GetGameMem()->persistantMempool);
    Effect_mem_slot_info                = NEW_ALLOCATION(312, effect_mem_slot_info);
    Effect_mem_slot_info->m_large_slots = NEW_ALLOCATION(314, effect_slot[MAX_LARGE_EFFECT_SLOTS]);
    Effect_mem_slot_info->m_small_slots = NEW_ALLOCATION(315, effect_slot[MAX_SMALL_EFFECT_SLOTS]);
    set_current_mempool(cur_mempool);
    Effect_mem_slot_info->m_total_large_slots      = MAX_LARGE_EFFECT_SLOTS;
    Effect_mem_slot_info->m_total_small_slots      = MAX_SMALL_EFFECT_SLOTS;
    Effect_mem_slot_info->m_large_effect_slot_size = LARGE_EFFECT_SLOT_SIZE;
    Effect_mem_slot_info->m_small_effect_slot_size = SMALL_EFFECT_SLOT_SIZE;
    Effect_mem_slot_info->m_used_large_slots       = 0;
    Effect_mem_slot_info->m_used_small_slots       = 0;
    Gamemem_info.ActivateSpellslotMempool();
    Effect_mem_slot_info->m_memory = Gamemem_info.GetSpellslotMempool()->destination;
    DEBUGASSERTLINE(328, Effect_mem_slot_info->m_memory);
    Gamemem_info.DeactivateSpellslotMempool();
    for (slot_cnt = 0; slot_cnt < MAX_LARGE_EFFECT_SLOTS; slot_cnt++) {
        Effect_mem_slot_info->m_large_slots[slot_cnt].m_slotMemory = Effect_mem_slot_info->m_memory + slot_cnt * LARGE_EFFECT_SLOT_SIZE;
        Effect_mem_slot_info->m_large_slots[slot_cnt].m_inUse      = false;
        Effect_mem_slot_info->m_large_slots[slot_cnt].m_slot_type  = 1;
        Effect_mem_slot_info->m_large_slots[slot_cnt].m_rheader    = 0;
        Effect_mem_slot_info->m_large_slots[slot_cnt].m_locked     = false;
    }
    for (slot_cnt = 0; slot_cnt < MAX_SMALL_EFFECT_SLOTS; slot_cnt++) {
        Effect_mem_slot_info->m_small_slots[slot_cnt].m_slotMemory =
            Effect_mem_slot_info->m_memory + slot_cnt * SMALL_EFFECT_SLOT_SIZE + 0xae000;
        Effect_mem_slot_info->m_small_slots[slot_cnt].m_inUse     = false;
        Effect_mem_slot_info->m_small_slots[slot_cnt].m_slot_type = 0;
        Effect_mem_slot_info->m_small_slots[slot_cnt].m_rheader   = 0;
        Effect_mem_slot_info->m_small_slots[slot_cnt].m_locked    = false;
    }
}

int effect::FreeEffectSlot(effect_slot *slot) {
    r_header *pRheader;

    DEBUGASSERTLINE(374, slot);
    DEBUGASSERTLINE(375, slot->m_rheader);
    DEBUGASSERTLINE(376, Gamemem_info.spellslot_mempool_is_active());
    DEBUGASSERTLINE(377, !slot->m_locked);
    pRheader        = slot->m_rheader;
    slot->m_inUse   = false;
    slot->m_rheader = 0;

    switch (slot->m_slot_type) {
    case 0:
        Effect_mem_slot_info->m_used_small_slots--;
        break;
    case 1:
        Effect_mem_slot_info->m_used_large_slots--;
        break;
    default:
        DEBUGERRORLINE(392, "Bad effect slot type!\n");
        break;
    }
    FreeEffect(pRheader);
    return 0;
}

void effect::FreeAllEffectSlots() {
    u32 slot_cnt;
    DEBUGASSERTLINE(407, Gamemem_info.spellslot_mempool_is_active());
    for (slot_cnt = 0; slot_cnt < Effect_mem_slot_info->m_total_large_slots; slot_cnt++) {
        if (Effect_mem_slot_info->m_large_slots[slot_cnt].m_rheader) {
            DEBUGASSERTLINE(413, !Effect_mem_slot_info->m_large_slots[slot_cnt].m_rheader->m_reference_count);
            DEBUGASSERTLINE(414, !Effect_mem_slot_info->m_large_slots[slot_cnt].m_rheader->m_is_loading);
            FreeEffectSlot(&Effect_mem_slot_info->m_large_slots[slot_cnt]);
        }
    }
    DEBUGASSERTLINE(421, !Effect_mem_slot_info->m_used_large_slots);
    for (slot_cnt = 0; slot_cnt < Effect_mem_slot_info->m_total_small_slots; slot_cnt++) {
        if (Effect_mem_slot_info->m_small_slots[slot_cnt].m_rheader) {
            DEBUGASSERTLINE(425, !Effect_mem_slot_info->m_small_slots[slot_cnt].m_rheader->m_reference_count);
            DEBUGASSERTLINE(426, !Effect_mem_slot_info->m_small_slots[slot_cnt].m_rheader->m_is_loading);
            FreeEffectSlot(&Effect_mem_slot_info->m_small_slots[slot_cnt]);
        }
    }
    DEBUGASSERTLINE(433, !Effect_mem_slot_info->m_used_small_slots);
}

u8 *effect::LoadSpellslotVfx(u32 size, r_header *rheader) {
    u32 type;
    u32 file_size = 0;
    u32 slot_cnt;
    effect_slot *pEffectSlot;
    char *target_filename = NULL;

    if (FindTargetFromFilename(rheader->m_filename, &target_filename)) {
        File file;
        file.GetFile(target_filename, 2);
        file_size = OSRoundUp32B(file.GetFileSize(NULL, 0));
        file.CloseFile();
        size += file_size;
    }
    DEBUGASSERTLINE(463, Gamemem_info.spellslot_mempool_is_active());
    if (size > Effect_mem_slot_info->m_large_effect_slot_size) {
        type = 2;
    } else if (size > Effect_mem_slot_info->m_small_effect_slot_size) {
        type = 1;
    } else {
        type = 0;
    }
    switch (type) {
    case 0:
        for (type = 0; type < 2; type++) {
            for (slot_cnt = 0; slot_cnt < Effect_mem_slot_info->m_small_effect_slot_size; slot_cnt++) {
                if (type == 0) {
                    if (!Effect_mem_slot_info->m_small_slots[slot_cnt].m_inUse) {
                        pEffectSlot = &Effect_mem_slot_info->m_small_slots[slot_cnt];
                    }
                } else {
                    DEBUGASSERTLINE(497, Effect_mem_slot_info->m_small_slots[slot_cnt].m_rheader);
                    if (Effect_mem_slot_info->m_small_slots[slot_cnt].m_rheader->m_reference_count == 0 &&
                        Effect_mem_slot_info->m_small_slots[slot_cnt].m_rheader->m_is_loading == 0 &&
                        !Effect_mem_slot_info->m_small_slots[slot_cnt].m_locked) {
                        pEffectSlot = &Effect_mem_slot_info->m_small_slots[slot_cnt];
                        FreeEffectSlot(pEffectSlot);
                    }
                }
                if (pEffectSlot) {
                    pEffectSlot->m_inUse   = true;
                    pEffectSlot->m_rheader = rheader;
                    Effect_mem_slot_info->m_used_small_slots++;
                    if (target_filename) {
                        rheader->m_peg_id = gPackFileSystem.ReadFileFromPack(target_filename, 2, pEffectSlot->m_slotMemory, file_size);
                        DEBUGASSERTLINE(513, rheader->m_peg_id >= 0);
                    }
                    rheader->unk1C = pEffectSlot->m_slotMemory;
                    return pEffectSlot->m_slotMemory;
                }
            }
            return NULL;
        }
    case 1:
        for (type = 0; type < 2; type++) {
            for (slot_cnt = 0; slot_cnt < Effect_mem_slot_info->m_small_effect_slot_size; slot_cnt++) {
                if (type == 0) {
                    if (!Effect_mem_slot_info->m_large_slots[slot_cnt].m_inUse) {
                        pEffectSlot = &Effect_mem_slot_info->m_large_slots[slot_cnt];
                    }
                } else {
                    DEBUGASSERTLINE(540, Effect_mem_slot_info->m_large_slots[slot_cnt].m_rheader);
                    if (Effect_mem_slot_info->m_large_slots[slot_cnt].m_rheader->m_reference_count == 0 &&
                        Effect_mem_slot_info->m_large_slots[slot_cnt].m_rheader->m_is_loading == 0 &&
                        !Effect_mem_slot_info->m_large_slots[slot_cnt].m_locked) {
                        pEffectSlot = &Effect_mem_slot_info->m_large_slots[slot_cnt];
                        FreeEffectSlot(pEffectSlot);
                    }
                }
                if (pEffectSlot) {
                    pEffectSlot->m_inUse   = true;
                    pEffectSlot->m_rheader = rheader;
                    Effect_mem_slot_info->m_used_large_slots++;
                    if (target_filename) {
                        rheader->m_peg_id = gPackFileSystem.ReadFileFromPack(target_filename, 2, pEffectSlot->m_slotMemory, file_size);
                        DEBUGASSERTLINE(556, rheader->m_peg_id >= 0);
                    }
                    rheader->unk1C = pEffectSlot->m_slotMemory;
                    return pEffectSlot->m_slotMemory + file_size;
                }
            }
        }
        return NULL;
    case 2:
#ifdef DEBUG
        sprintf(string_buffer, "'%s',\nalong with it's PEG file\nare too big (%dk)\nNeeds to be less than %dk(SUBJECT TO CHANGE!)\n",
                rheader->m_filename, (size + 1023) / 1024, (Effect_mem_slot_info->m_large_effect_slot_size + 1023) / 1024);
        DEBUGERRORLINE(570, string_buffer);
#endif
        return NULL;
    default:
        DEBUGERRORLINE(576, "Code should never fall through to this!\n");
        return NULL;
    }
    return NULL;
}

void effect::Unknown2() {
    vector3 vec3_1;
    vector2 vec2_1;
    vector2 vec2_2;
    vector2 vec2_3;
    vector2 vec2_4;
    if (unkE8.elapsed()) {
        unkE8.SetTimeout(100);
        if ((unk92 >> 4 & 7) < 6) {
            vec3_1 = unkC8 -= unkC.vec1;
        }
    }
}
