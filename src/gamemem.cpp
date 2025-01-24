#include "game/gamemem.h"

bool gamemem_active;
bool lbl_80475560;

void GameMem::activateGamemem(void) {
    gamemem_active = true;
    allocateMempools();
}

void GameMem::allocateMempools(void) {
    lbl_80475560 = true;
    gHeapAlloc = true;
    u32 size;

    SetCurrentMempool(0);

    persistantMempool.data = AllocateArray(0x363800, "gamemem.cpp", 168);
    copy(&persistantMempool, persistantMempool.data, 0x363800, "persistant", 32);
    resetOffset(&persistantMempool);

    soundMempool.data = AllocateArray(0x10000, "gamemem.cpp", 174);
    copy(&soundMempool, soundMempool.data, 0x10000, "sound", 16);
    resetOffset(&soundMempool);

    size = 0; // Not the real size

    perlevelMempool.data = AllocateArray(size, "gamemem.cpp", 181);
    copy(&perlevelMempool, perlevelMempool.data, size, "perlevel", 16);
    resetOffset(&perlevelMempool);
    SetCurrentMempool(&perlevelMempool);

    cutsceneMempool.data = AllocateArray(0x234000, "gamemem.cpp", 191);
    copy(&cutsceneMempool, cutsceneMempool.data, 0x234000, "cutscene", 16);
    resetOffset(&cutsceneMempool);
    SetCurrentMempool(&cutsceneMempool);

    summonMempool.data = AllocateArray(0xf0000, "gamemem.cpp", 108);
    copy(&summonMempool, summonMempool.data, 0xf0000, "summon", 32);

    spellslotMempool.data = AllocateArray(0x144000, "gamemem.cpp", 202);
    copy(&spellslotMempool, spellslotMempool.data, 0x144000, "spellslot", 16);
    perlevel_mempool_active = false;
    cutscene_mempool_active = false;
    summon_mempool_active = false;
    spellslot_mempool_active = false;
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
