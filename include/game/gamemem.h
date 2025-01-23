#include "game/memory.h"

extern volatile bool gHeapAlloc;

class GameMem {
    Memory persistantMempool;
    Memory soundMempool;
    Memory perlevelMempool;
    Memory cutsceneMempool;
    Memory spellslotMempool;
    Memory summonMempool;
    bool perlevel_mempool_active;
    bool cutscene_mempool_active;
    bool summon_mempool_active;
    bool spellslot_mempool_active;
    bool persistant_mempool_active;

    void activateGamemem(void);
    void allocateMempools();
};
