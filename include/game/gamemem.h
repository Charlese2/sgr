#include "game/console.h"
#include "game/memory.h"
#include "game/MemSystem.h"

extern bool gHeapAlloc;

class GameMem {
    public:
    Mempool persistantMempool;
    char* persistant_data;
    Mempool soundMempool;
    char* sound_data;
    Mempool perlevelMempool;
    char* perlevel_data;
    Mempool cutsceneMempool;
    char* cutscene_data;
    Mempool spellslotMempool;
    char* spellslot_data;
    Mempool summonMempool;
    char* summon_data;
    bool perlevel_mempool_active;
    bool cutscene_mempool_active;
    bool summon_mempool_active;
    bool spellslot_mempool_active;
    bool persistant_mempool_active;

    console_command toggle_gamemem_prints;
    console_command toggle_mempool_usage;

    GameMem();

    static bool toggle_show_gamemem_prints(void);
    static bool toggle_show_mempool_usage(void);
    int ActivateGamemem(void);
    bool AllocateMempools(void);
    void ActivateSummonMempool(void);
    void DeactivateSummonMempool(void);
    bool summon_mempool_is_active(void);
    Mempool * GetSummonMempool(void);
    void ActivateCutsceneMempool(void);
    void DeactivateCutsceneMempool(void);
    bool cutscene_mempool_is_active(void);
    Mempool * GetCutsceneMempool(void);
    void ActivateSpellslotMempool(void);
    void DeactivateSpellslotMempool(void);
    bool spellslot_mempool_is_active(void);
    Mempool * GetSpellslotMempool(void);
    void ActivatePerlevelMempool(void);
    void DeactivatePerlevelMempool(void);
    bool perlevel_mempool_is_active(void);
    Mempool * GetPerlevelMempool(void);
    void ClearPerlevelMempool(void);
    void ActivatePersistantMempool(void);
    GameMem * GetGameMem(void);
    Mempool * GetSoundMempool(void);
    void DisplayMempoolUsage();
};

extern GameMem Gamemem_info;

inline void ResetOffset(Mempool * pool) {
    pool->offset = 0;
}

inline u32 getPoolSpaceLeft(Mempool * pool) {
    return pool->size - pool->offset;
}
