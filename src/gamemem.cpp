#include "game/gamemem.h"
#include "game/console.h"
#include "dolphin/os/OSAlloc.h"
#include <stdio.h>

bool gamemem_active;
GameMem gGameMem;
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

    gGameMem.persistantMempool.data = AllocateArray(0x363800, "gamemem.cpp", 168);
    Copy(&gGameMem.persistantMempool, gGameMem.persistantMempool.data, 0x363800, "persistant", 32);
    ResetOffset(&gGameMem.persistantMempool);

    gGameMem.soundMempool.data = AllocateArray(0x10000, "gamemem.cpp", 174);
    Copy(&gGameMem.soundMempool, gGameMem.soundMempool.data, 0x10000, "sound", 16);
    ResetOffset(&gGameMem.soundMempool);

    heap = GetHeapHandle(&gMemSystem);
    size = OSCheckHeap(heap) - 0x1dc500;
    printf("Allocating %.2f KB for the perlevel mempool\n", size / 1024.0f );

    gGameMem.perlevelMempool.data = AllocateArray(size, "gamemem.cpp", 181);
    Copy(&gGameMem.perlevelMempool, gGameMem.perlevelMempool.data, size, "perlevel", 16);
    ResetOffset(&gGameMem.perlevelMempool);
    SetCurrentMempool(&gGameMem.perlevelMempool);

    gGameMem.cutsceneMempool.data = AllocateArray(0x234000, "gamemem.cpp", 191);
    Copy(&gGameMem.cutsceneMempool, gGameMem.cutsceneMempool.data, 0x234000, "cutscene", 16);
    ResetOffset(&gGameMem.cutsceneMempool);
    SetCurrentMempool(&gGameMem.cutsceneMempool);

    gGameMem.summonMempool.data = AllocateArray(0xf0000, "gamemem.cpp", 198);
    Copy(&gGameMem.summonMempool, gGameMem.summonMempool.data, 0xf0000, "summon", 32);

    gGameMem.spellslotMempool.data = AllocateArray(0x144000, "gamemem.cpp", 202);
    Copy(&gGameMem.spellslotMempool, gGameMem.spellslotMempool.data, 0x144000, "spellslot", 16);

    gGameMem.perlevel_mempool_active = false;
    gGameMem.cutscene_mempool_active = false;
    gGameMem.summon_mempool_active = false;
    gGameMem.spellslot_mempool_active = false;

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
