#include "game/gamemem.h"
#include "game/console.h"
#include "dolphin/os/OSAlloc.h"
#include <stdio.h>

bool gamemem_active;
GameMem Gamemem_info;
extern MemSystem gMemSystem;

static console_command show_game_mem_prints;

GameMem::GameMem() {
#ifdef DEBUG
    register_command(&show_game_mem_prints, "show_game_mem_prints", "Toggles Show_game_mem_prints", TOGGLE, GameMem::toggleShowGameMemPrints);
#endif
}

int GameMem::ActivateGamemem(void) {
    gamemem_active = true;
    AllocateMempools();
    return 0;
}

void GameMem::AllocateMempools(void) {
    gHeapAlloc = true;
    u32 size;
    int heap;

    SetCurrentMempool(0);

    Gamemem_info.persistantMempool.data = AllocateArray(0x363800, "gamemem.cpp", 168);
    Copy(&Gamemem_info.persistantMempool, Gamemem_info.persistantMempool.data, 0x363800, "persistant", 32);
    ResetOffset(&Gamemem_info.persistantMempool);

    Gamemem_info.soundMempool.data = AllocateArray(0x10000, "gamemem.cpp", 174);
    Copy(&Gamemem_info.soundMempool, Gamemem_info.soundMempool.data, 0x10000, "sound", 16);
    ResetOffset(&Gamemem_info.soundMempool);

    heap = GetHeapHandle(&gMemSystem);
    size = OSCheckHeap(heap) - 0x1dc500;
    printf("Allocating %.2f KB for the perlevel mempool\n", size / 1024.0f );

    Gamemem_info.perlevelMempool.data = AllocateArray(size, "gamemem.cpp", 181);
    Copy(&Gamemem_info.perlevelMempool, Gamemem_info.perlevelMempool.data, size, "perlevel", 16);
    ResetOffset(&Gamemem_info.perlevelMempool);
    SetCurrentMempool(&Gamemem_info.perlevelMempool);

    Gamemem_info.cutsceneMempool.data = AllocateArray(0x234000, "gamemem.cpp", 191);
    Copy(&Gamemem_info.cutsceneMempool, Gamemem_info.cutsceneMempool.data, 0x234000, "cutscene", 16);
    ResetOffset(&Gamemem_info.cutsceneMempool);
    SetCurrentMempool(&Gamemem_info.cutsceneMempool);

    Gamemem_info.summonMempool.data = AllocateArray(0xf0000, "gamemem.cpp", 198);
    Copy(&Gamemem_info.summonMempool, Gamemem_info.summonMempool.data, 0xf0000, "summon", 32);

    Gamemem_info.spellslotMempool.data = AllocateArray(0x144000, "gamemem.cpp", 202);
    Copy(&Gamemem_info.spellslotMempool, Gamemem_info.spellslotMempool.data, 0x144000, "spellslot", 16);

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

bool GameMem::GetSummonMempoolActive(void) {
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

bool GameMem::GetCutsceneMempoolActive(void) {
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

bool GameMem::GetSpellslotMempoolActive(void) {
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

bool GameMem::GetPerlevelMempoolActive(void) {
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

GameMem * GameMem::GetGameMem(void) {
    return this;
}

Memory * GameMem::GetSoundMempool(void) {
    return &soundMempool;
}

u32 GetHeapHandle(MemSystem * memSystem) {
    return memSystem->heapHandle;
}
