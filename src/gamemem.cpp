#include "game/gamemem.h"

bool gamemem_active;
bool lbl_80475560;

void GameMem::activateGamemem() {
    gamemem_active = true;
    allocateMempools();
}

void GameMem::allocateMempools() {
    lbl_80475560 = true;
    gHeapAlloc = true;
    SetCurrentMempool(0);
    
}
