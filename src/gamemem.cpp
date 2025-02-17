#include "game/gamemem.h"
#include "game/console.h"
#include "dolphin/os/OSAlloc.h"
#include "dolphin/os.h"
#include "macros.h"
#include <stdio.h>

bool gamemem_active;
bool show_game_mem_prints;
bool show_mempool_usage;
GameMem Gamemem_info;
extern MemSystem gMemSystem;

static console_command toggle_show_game_mem_prints;
static console_command toggle_show_mempool_usage;

GameMem::GameMem() {
#ifdef DEBUG
    register_command(&toggle_show_game_mem_prints, "show_game_mem_prints", "Toggles Show_game_mem_prints", TOGGLE, Gamemem_info.ToggleShowGameMemPrints);
    register_command(&toggle_show_mempool_usage, "show_mempool_usage", "Toggles bShowMempoolUsage", TOGGLE, Gamemem_info.ToggleShowMempoolUsage);
#endif
}

bool GameMem::ToggleShowGameMemPrints(void) {

}

bool GameMem::ToggleShowMempoolUsage(void) {

}

int GameMem::ActivateGamemem(void) {
    ASSERTLINE(144, !gamemem_active);
#ifdef DEBUG
    if (!show_game_mem_prints) {
        printf("GAME_MEM: init\n");
    }
#endif
    gamemem_active = true;
    AllocateMempools();
    return 0;
}

void GameMem::AllocateMempools(void) {
    gHeapAlloc = true;
    u32 size;
    int heap;

    SetCurrentMempool(0);

    Gamemem_info.persistant_data = AllocateArray(0x363800, "gamemem.cpp", 168);
    Copy(&Gamemem_info.persistantMempool, Gamemem_info.persistant_data, 0x363800, "persistant", 32);
    ResetOffset(&Gamemem_info.persistantMempool);

    Gamemem_info.sound_data = AllocateArray(0x10000, "gamemem.cpp", 174);
    Copy(&Gamemem_info.soundMempool, Gamemem_info.sound_data, 0x10000, "sound", 16);
    ResetOffset(&Gamemem_info.soundMempool);

    heap = GetHeapHandle(&gMemSystem);
    size = OSCheckHeap(heap) - 0x1dc500;
    printf("Allocating %.2f KB for the perlevel mempool\n", size / 1024.0f );

    Gamemem_info.perlevel_data = AllocateArray(size, "gamemem.cpp", 181);
    Copy(&Gamemem_info.perlevelMempool, Gamemem_info.perlevel_data, size, "perlevel", 16);
    ResetOffset(&Gamemem_info.perlevelMempool);
    SetCurrentMempool(&Gamemem_info.perlevelMempool);

    Gamemem_info.cutscene_data = AllocateArray(0x234000, "gamemem.cpp", 191);
    Copy(&Gamemem_info.cutsceneMempool, Gamemem_info.cutscene_data, 0x234000, "cutscene", 16);
    ResetOffset(&Gamemem_info.cutsceneMempool);
    SetCurrentMempool(&Gamemem_info.cutsceneMempool);

    Gamemem_info.summon_data = AllocateArray(0xf0000, "gamemem.cpp", 198);
    Copy(&Gamemem_info.summonMempool, Gamemem_info.summon_data, 0xf0000, "summon", 32);

    Gamemem_info.spellslot_data = AllocateArray(0x144000, "gamemem.cpp", 202);
    Copy(&Gamemem_info.spellslotMempool, Gamemem_info.spellslot_data, 0x144000, "spellslot", 16);

    Gamemem_info.perlevel_mempool_active = false;
    Gamemem_info.cutscene_mempool_active = false;
    Gamemem_info.summon_mempool_active = false;
    Gamemem_info.spellslot_mempool_active = false;

    SetCurrentMempool(0);
    gHeapAlloc = false;
}

void GameMem::ActivateSummonMempool(void) {
    summon_mempool_active = true;
    ResetOffset(&summonMempool);
}

void GameMem::DeactivateSummonMempool(void) {
    summon_mempool_active = false;
}

bool GameMem::summon_mempool_is_active(void) {
    return summon_mempool_active;
}

Memory * GameMem::GetSummonMempool(void) {
    return &summonMempool;
}

void GameMem::ActivateCutsceneMempool(void) {
    cutscene_mempool_active = true;
    ResetOffset(&cutsceneMempool);
}

void GameMem::DeactivateCutsceneMempool(void) {
    cutscene_mempool_active = false;
}

bool GameMem::cutscene_mempool_is_active(void) {
    return cutscene_mempool_active;
}

Memory * GameMem::GetCutsceneMempool(void) {
    return &cutsceneMempool;
}

void GameMem::ActivateSpellslotMempool(void) {
    spellslot_mempool_active = true;
    ResetOffset(&spellslotMempool);
}

void GameMem::DeactivateSpellslotMempool(void) {
    spellslot_mempool_active = false;
}

bool GameMem::spellslot_mempool_is_active(void) {
    return spellslot_mempool_active;
}

Memory * GameMem::GetSpellslotMempool(void) {
    return &spellslotMempool;
}

void GameMem::ActivatePerlevelMempool(void) {
    perlevel_mempool_active = true;
    ResetOffset(&perlevelMempool);
    AllocateInPool(&perlevelMempool, 0x234000);
}

void GameMem::DeactivatePerlevelMempool(void) {
    perlevel_mempool_active = false;
}

bool GameMem::perlevel_mempool_is_active(void) {
    return perlevel_mempool_active;
}

Memory * GameMem::GetPerlevelMempool(void) {
    return &perlevelMempool;
}

void GameMem::ClearPerlevelMempool(void) {
    ResetOffset(&perlevelMempool);
    AllocateInPool(&perlevelMempool, 0x234000);
}

void GameMem::ActivatePersistantMempool(void) {
    persistant_mempool_active = true;
    ResetOffset(&persistantMempool);
}

Memory * GameMem::GetSoundMempool(void) {
    return &soundMempool;
}

u32 GetHeapHandle(MemSystem * memSystem) {
    return memSystem->heapHandle;
}
