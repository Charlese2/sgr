#include "game/gamemem.h"
#include "game/gr.h"
#include "game/gr_font.h"
#include "game/console.h"
#include "game/characterinfo.h"
#include "game/effect_mem.h"
#include "game/debug.h"
#include "dolphin/os.h"
#include "game/macros.h"
#include "game/memory.h"
#include "macros.h"
#include <stdio.h>

GameMem Gamemem_info;
bool Show_game_mem_prints;
bool bShowMempoolUsage;
bool gamemem_active;

#ifdef DEBUG
console_command toggle_gamemem_prints("show_game_mem_prints", "Toggles Show_game_mem_prints", TOGGLE,
                                      Gamemem_info.toggle_show_gamemem_prints);
console_command toggle_mempool_usage("show_mempool_usage", "Toggles bShowMempoolUsage", TOGGLE, Gamemem_info.toggle_show_mempool_usage);
#endif

inline GameMem::GameMem() {}

#ifdef DEBUG
int GameMem::toggle_show_gamemem_prints(void) {
    if (calling_a_command_function) {
        process_command(193);
        if ((arg_type & 0x40)) {
            Show_game_mem_prints = true;
        } else if (arg_type & 0x80) {
            Show_game_mem_prints = false;
        } else if ((arg_type & 1)) {
            Show_game_mem_prints ^= true;
        }
    }

    if (doing_help_for_comand) {
        sprintf(string_buffer, "Usage: %s [bool]\nSets %s to true or false. If nothing passed, then toggles it\n", "show_game_mem_prints",
                "Show_game_mem_prints");
        print_to_console(string_buffer, false);
    }

    if (checking_status_for_command) {
        sprintf(string_buffer, "%s is %s\n", "show_game_mem_prints", Show_game_mem_prints ? "TRUE" : "FALSE");
        print_to_console(string_buffer, false);
    }

    return Show_game_mem_prints;
}

int GameMem::toggle_show_mempool_usage(void) {
    if (calling_a_command_function) {
        process_command(193);
        if ((arg_type & 0x40)) {
            bShowMempoolUsage = true;
        } else if (arg_type & 0x80) {
            bShowMempoolUsage = false;
        } else if ((arg_type & 1)) {
            bShowMempoolUsage ^= true;
        }
    }

    if (doing_help_for_comand) {
        sprintf(string_buffer, "Usage: %s [bool]\nSets %s to true or false. If nothing passed, then toggles it\n", "show_mempool_usage",
                "bShowMempoolUsage");
        print_to_console(string_buffer, false);
    }

    if (checking_status_for_command) {
        sprintf(string_buffer, "%s is %s\n", "show_mempool_usage", bShowMempoolUsage ? "TRUE" : "FALSE");
        print_to_console(string_buffer, false);
    }

    return bShowMempoolUsage;
}
#endif

int GameMem::ActivateGamemem(void) {
    ASSERTLINE(144, !gamemem_active);
#if DEBUG
    if (Show_game_mem_prints) {
        printf("GAME_MEM: init\n");
    }
#endif
    gamemem_active = true;
    Gamemem_info.AllocateMempools();
    return 0;
}

inline bool GameMem::AllocateMempools(void) {
    u32 size;
    int heap;

    ASSERTLINE(159, gamemem_active);
#if DEBUG
    if (Show_game_mem_prints) {
        printf("GAME_MEM: allocate mempools\n");
    }
#endif

    gHeapAlloc = true;
    set_current_mempool(0);

    persistant_data = (u8 *)operator new[](0x363800, "gamemem.cpp", 168);
    DEBUGASSERTLINE(169, persistant_data);
    persistantMempool.Copy(persistant_data, 0x363800, "persistant", 32);
    persistantMempool.ResetOffset();

    sound_data = (u8 *)operator new[](0x10000, "gamemem.cpp", 174);
    soundMempool.Copy(sound_data, 0x10000, "sound", 16);
    soundMempool.ResetOffset();

    size = OSCheckHeap(gMemSystem.GetHeapHandle()) - 0x1dc500;
    printf("Allocating %.2f KB for the perlevel mempool\n", size / 1024.0f);

    perlevel_data = (u8 *)operator new[](size, "gamemem.cpp", 181);
    DEBUGASSERTLINE(182, perlevel_data);
    perlevelMempool.Copy(perlevel_data, size, "perlevel", 16);
    perlevelMempool.ResetOffset();
    set_current_mempool(&perlevelMempool);

    cutscene_data = (u8 *)operator new[](0x234000, "gamemem.cpp", 191);
    DEBUGASSERTLINE(192, cutscene_data);
    cutsceneMempool.Copy(cutscene_data, 0x234000, "cutscene", 16);
    cutsceneMempool.ResetOffset();
    set_current_mempool(&cutsceneMempool);

    summon_data = (u8 *)operator new[](0xf0000, "gamemem.cpp", 198);
    DEBUGASSERTLINE(199, summon_data);
    summonMempool.Copy(summon_data, 0xf0000, "summon", 32);

    spellslot_data = (u8 *)operator new[](0x144000, "gamemem.cpp", 202);
    DEBUGASSERTLINE(203, spellslot_data);
    spellslotMempool.Copy(spellslot_data, 0x144000, "spellslot", 16);

    perlevel_mempool_active  = false;
    cutscene_mempool_active  = false;
    summon_mempool_active    = false;
    spellslot_mempool_active = false;

    set_current_mempool(0);
    gHeapAlloc = false;
    return false;
}

void GameMem::ActivateSummonMempool(void) {
    DEBUGPRINT("GAME_MEM: activate summon mempool\n", Show_game_mem_prints);
    DEBUGASSERTLINE(228, gamemem_active);
    DEBUGASSERTLINE(229, !cutscene_mempool_active);
    DEBUGASSERTLINE(230, !summon_mempool_active);
    summon_mempool_active = true;
    summonMempool.ResetOffset();
}

void GameMem::DeactivateSummonMempool(void) {
    DEBUGPRINT("GAME_MEM: deactivate summon mempool\n", Show_game_mem_prints);
    DEBUGASSERTLINE(244, gamemem_active);
    DEBUGASSERTLINE(245, summon_mempool_active);
    summon_mempool_active = false;
}

bool GameMem::summon_mempool_is_active(void) {
    DEBUGASSERTLINE(257, gamemem_active);
    return summon_mempool_active;
}

Mempool *GameMem::GetSummonMempool(void) {
    DEBUGASSERTLINE(269, gamemem_active);
    DEBUGASSERTLINE(270, summon_mempool_active);
    return &summonMempool;
}

void GameMem::ActivateCutsceneMempool(void) {
    DEBUGPRINT("GAME_MEM: activate cutscene mempool\n", Show_game_mem_prints);
    DEBUGASSERTLINE(284, gamemem_active);
    DEBUGASSERTLINE(285, !summon_mempool_active);
    DEBUGASSERTLINE(286, !spellslot_mempool_active);
    DEBUGASSERTLINE(287, !cutscene_mempool_active);
    cutscene_mempool_active = true;
    cutsceneMempool.ResetOffset();
}

void GameMem::DeactivateCutsceneMempool(void) {
    DEBUGPRINT("GAME_MEM: deactivate cutscene mempool\n", Show_game_mem_prints);
    DEBUGASSERTLINE(301, gamemem_active);
    DEBUGASSERTLINE(302, cutscene_mempool_active);
    cutscene_mempool_active = false;
}

bool GameMem::cutscene_mempool_is_active(void) {
    DEBUGASSERTLINE(314, gamemem_active);
    return cutscene_mempool_active;
}

Mempool *GameMem::GetCutsceneMempool(void) {
    DEBUGASSERTLINE(326, gamemem_active);
    DEBUGASSERTLINE(327, cutscene_mempool_active);
    return &cutsceneMempool;
}

void GameMem::ActivateSpellslotMempool(void) {
    DEBUGPRINT("GAME_MEM: activate spellslot mempool\n", Show_game_mem_prints);
    DEBUGASSERTLINE(341, gamemem_active);
    DEBUGASSERTLINE(342, !cutscene_mempool_active);
    DEBUGASSERTLINE(343, !spellslot_mempool_active);
    spellslot_mempool_active = true;
    spellslotMempool.ResetOffset();
}

void GameMem::DeactivateSpellslotMempool(void) {
    DEBUGPRINT("GAME_MEM: deactivate spellslot mempool\n", Show_game_mem_prints);
    DEBUGASSERTLINE(357, gamemem_active);
    DEBUGASSERTLINE(358, spellslot_mempool_active);
    spellslot_mempool_active = false;
}

bool GameMem::spellslot_mempool_is_active(void) {
    DEBUGASSERTLINE(370, gamemem_active);
    return spellslot_mempool_active;
}

Mempool *GameMem::GetSpellslotMempool(void) {
    DEBUGASSERTLINE(382, gamemem_active);
    DEBUGASSERTLINE(383, spellslot_mempool_active);
    return &spellslotMempool;
}

void GameMem::ActivatePerlevelMempool(void) {
    DEBUGPRINT("GAME_MEM: activate perlevel mempool\n", Show_game_mem_prints);
    DEBUGASSERTLINE(397, gamemem_active);
    DEBUGASSERTLINE(398, !perlevel_mempool_active);
    perlevel_mempool_active = true;
    perlevelMempool.ResetOffset();
    allocate_in_mempool(&perlevelMempool, 0x234000);
}

void GameMem::DeactivatePerlevelMempool(void) {
    DEBUGPRINT("GAME_MEM: deactivate perlevel mempool\n", Show_game_mem_prints);
    DEBUGASSERTLINE(414, gamemem_active);
    DEBUGASSERTLINE(415, perlevel_mempool_active);
    perlevel_mempool_active = false;
}

bool GameMem::perlevel_mempool_is_active(void) {
    DEBUGASSERTLINE(427, gamemem_active);
    return perlevel_mempool_active;
}

Mempool *GameMem::GetPerlevelMempool(void) {
    DEBUGASSERTLINE(439, gamemem_active);
    DEBUGASSERTLINE(440, perlevel_mempool_active);
    return &perlevelMempool;
}

void GameMem::ClearPerlevelMempool(void) {
    DEBUGPRINT("GAME_MEM: clear perlevel mempool\n", Show_game_mem_prints);
    DEBUGASSERTLINE(453, gamemem_active);
    DEBUGASSERTLINE(454, perlevel_mempool_active);
    perlevelMempool.ResetOffset();
    allocate_in_mempool(&perlevelMempool, 0x234000);
}

void GameMem::ActivatePersistantMempool(void) {
    DEBUGPRINT("GAME_MEM: activate persistant mempool\n", Show_game_mem_prints);
    DEBUGASSERTLINE(469, gamemem_active);
    persistant_mempool_active = true;
    persistantMempool.ResetOffset();
}

GameMem *GameMem::GetGameMem(void) {
    DEBUGASSERTLINE(508, gamemem_active);
    DEBUGASSERTLINE(509, persistant_mempool_active);
    return this;
}

Mempool *GameMem::GetSoundMempool(void) { return &soundMempool; }

#ifdef DEBUG
void GameMem::DisplayMempoolUsage() {
    char buffer[248];
    Mempool *mempool;
    if (bShowMempoolUsage) {
        mempool = (Mempool *)Gamemem_info.GetGameMem();
        sprintf(buffer, "Persistant: %5dKB of %5dKB in use\n", mempool->offset >> 10, mempool->size >> 10);
        gr_font::DrawTextOnScreen2D(10, 10, buffer, -1);

        mempool = Gamemem_info.GetPerlevelMempool();
        sprintf(buffer, "Per Level:  %5dKB of %5dKB in use\n", mempool->offset >> 10, mempool->size >> 10);
        gr_font::draw_mempool_info_on_new_line(buffer, -1);

        if (Gamemem_info.cutscene_mempool_is_active()) {
            mempool = Gamemem_info.GetCutsceneMempool();
            sprintf(buffer, "Cutscene:   %5dKB of %5dKB in use\n", mempool->offset >> 10, mempool->size >> 10);
            gr_font::draw_mempool_info_on_new_line(buffer, -1);
        } else if (Gamemem_info.summon_mempool_is_active()) {
            mempool = Gamemem_info.GetSummonMempool();
            sprintf(buffer, "Summon:     %5dKB of %5dKB in use\n", mempool->offset >> 10, mempool->size >> 10);
            gr_font::draw_mempool_info_on_new_line(buffer, -1);
        }
        DisplayCharacterMeminfo(buffer);
    }
}
#endif
