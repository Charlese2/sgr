#include "game/gamemem.h"
#include "dolphin/os/OSAlloc.h"

bool gamemem_active;
extern Memory gMemSystem;

int GameMem::activateGamemem(void) {
    gamemem_active = true;
    allocateMempools();
    return 0;
}

void GameMem::allocateMempools(void) {
    gHeapAlloc = true;
    u32 size;
    int heap;

    SetCurrentMempool(0);

    gGameMem.persistantMempool.data = AllocateArray(0x363800, "gamemem.cpp", 168);
    copy(&gGameMem.persistantMempool, gGameMem.persistantMempool.data, 0x363800, "persistant", 32);
    resetOffset(&gGameMem.persistantMempool);

    gGameMem.soundMempool.data = AllocateArray(0x10000, "gamemem.cpp", 174);
    copy(&gGameMem.soundMempool, gGameMem.soundMempool.data, 0x10000, "sound", 16);
    resetOffset(&gGameMem.soundMempool);

    heap = getPoolSize(&gMemSystem);
    size = OSCheckHeap(heap) - 0x1dc500;
    printf("Allocating %.2f KB for the perlevel mempool\n", size - (float)4503599627370496.0 * 0.0009765625f);

    gGameMem.perlevelMempool.data = AllocateArray(size, "gamemem.cpp", 181);
    copy(&gGameMem.perlevelMempool, gGameMem.perlevelMempool.data, size, "perlevel", 16);
    resetOffset(&gGameMem.perlevelMempool);
    SetCurrentMempool(&gGameMem.perlevelMempool);

    gGameMem.cutsceneMempool.data = AllocateArray(0x234000, "gamemem.cpp", 191);
    copy(&gGameMem.cutsceneMempool, gGameMem.cutsceneMempool.data, 0x234000, "cutscene", 16);
    resetOffset(&gGameMem.cutsceneMempool);
    SetCurrentMempool(&gGameMem.cutsceneMempool);

    gGameMem.summonMempool.data = AllocateArray(0xf0000, "gamemem.cpp", 198);
    copy(&gGameMem.summonMempool, gGameMem.summonMempool.data, 0xf0000, "summon", 32);

    gGameMem.spellslotMempool.data = AllocateArray(0x144000, "gamemem.cpp", 202);
    copy(&gGameMem.spellslotMempool, gGameMem.spellslotMempool.data, 0x144000, "spellslot", 16);

    gGameMem.perlevel_mempool_active = false;
    gGameMem.cutscene_mempool_active = false;
    gGameMem.summon_mempool_active = false;
    gGameMem.spellslot_mempool_active = false;

    SetCurrentMempool(0);
    gHeapAlloc = false;
}

void GameMem::activateSummonMempool(void) {
    summon_mempool_active = true;
    resetOffset(&summonMempool);
}

void GameMem::deactivateSummonMempool(void) {
    summon_mempool_active = false;
}

bool GameMem::getSummonMempoolActive(void) {
    return summon_mempool_active;
}

Memory * GameMem::getSummonMempool(void) {
    return &summonMempool;
}

void GameMem::activateCutsceneMempool(void) {
    cutscene_mempool_active = true;
    resetOffset(&cutsceneMempool);
}

void GameMem::deactivateCutsceneMempool(void) {
    cutscene_mempool_active = false;
}

bool GameMem::getCutsceneMempoolActive(void) {
    return cutscene_mempool_active;
}

Memory * GameMem::getCutsceneMempool(void) {
    return &cutsceneMempool;
}

void GameMem::activateSpellslotMempool(void) {
    spellslot_mempool_active = true;
    resetOffset(&spellslotMempool);
}

void GameMem::deactivateSpellslotMempool(void) {
    spellslot_mempool_active = false;
}

bool GameMem::getSpellslotMempoolActive(void) {
    return spellslot_mempool_active;
}

Memory * GameMem::getSpellslotMempool(void) {
    return &spellslotMempool;
}

void GameMem::activatePerlevelMempool(void) {
    perlevel_mempool_active = true;
    resetOffset(&perlevelMempool);
    allocateInPool(&perlevelMempool, 0x234000);
}

void GameMem::deactivatePerlevelMempool(void) {
    perlevel_mempool_active = false;
}

bool GameMem::getPerlevelMempoolActive(void) {
    return perlevel_mempool_active;
}

Memory * GameMem::getPerlevelMempool(void) {
    return &perlevelMempool;
}

void GameMem::clearPerlevelMempool(void) {
    resetOffset(&perlevelMempool);
    allocateInPool(&perlevelMempool, 0x234000);
}

void GameMem::activatePersistantMempool(void) {
    persistant_mempool_active = true;
    resetOffset(&persistantMempool);
}

GameMem * GameMem::getGameMem(void) {
    return this;
}

Memory * GameMem::getSoundMempool(void) {
    return &soundMempool;
}
