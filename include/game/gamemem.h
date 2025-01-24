#include "game/memory.h"

extern volatile bool gHeapAlloc;



class GameMem {
    public:
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

    void toggleShowGameMemPrints(void);
    bool toggleShowMempoolUsage(void);
    void activateGamemem(void);
    void allocateMempools(void);
    void activateSummonMempool(void);
    void deactivateSummonMempool(void);
    bool getSummonMempoolActive(void);
    Memory * getSummonMempool(void);
    void activateCutsceneMempool(void);
    void deactivateCutsceneMempool(void);
    bool getCutsceneMempoolActive(void);
    Memory * getCutsceneMempool(void);
    void activateSpellslotMempool(void);
    void deactivateSpellslotMempool(void);
    bool getSpellslotMempoolActive(void);
    Memory * getSpellslotMempool(void);
    void activatePerlevelMempool(void);
    void deactivatePerlevelMempool(void);
    bool getPerlevelMempoolActive(void);
    Memory * getPerlevelMempool(void);
    void clearPerlevelMempool(void);
    void activatePersistantMempool(void);
    GameMem * getGameMem(void);
    Memory * getSoundMempool(void);
};

void resetOffset(Memory * mempool) {
    mempool->offset = 0;
}

u32 getPoolSize(Memory * pool) {
    return pool->size;
}
