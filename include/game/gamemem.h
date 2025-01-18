#include "game/memory.h"

extern volatile bool gHeapAlloc;

class GameMem {
    Memory memory[6];
    char unkD8[2];
    bool summon_mempool_active;
    bool spellslot_mempool_active;
    bool persistant_mempool_active;
    bool unkDD;
    bool unkDE;
    bool unkDF;
    bool unkE0;
    bool unkE1;
    bool unkE2;
    bool unkE3;
    bool unkE4;
    bool unkE5;
    bool unkE6;
    bool unkE7;
    bool unkE8;
    bool unkE9;
    bool unkEA;
    bool unkEB;
    bool unkEC;
    bool unkED;
    bool unkEE;
    bool unkEF;

    void activateGamemem(void);
    void allocateMempools();
};
