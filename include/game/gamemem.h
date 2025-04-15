#include "game/memory.h"
#include "game/MemSystem.h"

extern bool gHeapAlloc;

class GameMem {
    public:
    Memory persistantMempool;
    char* persistant_data;
    Memory soundMempool;
    char* sound_data;
    Memory perlevelMempool;
    char* perlevel_data;
    Memory cutsceneMempool;
    char* cutscene_data;
    Memory spellslotMempool;
    char* spellslot_data;
    Memory summonMempool;
    char* summon_data;
    bool perlevel_mempool_active;
    bool cutscene_mempool_active;
    bool summon_mempool_active;
    bool spellslot_mempool_active;
    bool persistant_mempool_active;

    GameMem();

    static bool ToggleShowGameMemPrints(void);
    static bool ToggleShowMempoolUsage(void);
    int ActivateGamemem(void);
    void AllocateMempools(void);
    void ActivateSummonMempool(void);
    void DeactivateSummonMempool(void);
    bool summon_mempool_is_active(void);
    Memory * GetSummonMempool(void);
    void ActivateCutsceneMempool(void);
    void DeactivateCutsceneMempool(void);
    bool cutscene_mempool_is_active(void);
    Memory * GetCutsceneMempool(void);
    void ActivateSpellslotMempool(void);
    void DeactivateSpellslotMempool(void);
    bool spellslot_mempool_is_active(void);
    Memory * GetSpellslotMempool(void);
    void ActivatePerlevelMempool(void);
    void DeactivatePerlevelMempool(void);
    bool perlevel_mempool_is_active(void);
    Memory * GetPerlevelMempool(void);
    void ClearPerlevelMempool(void);
    void ActivatePersistantMempool(void);
    GameMem * GetGameMem(void);
    Memory * GetSoundMempool(void);
    void DisplayMempoolUsage();
};

u32 GetHeapHandle(MemSystem * memSystem);

extern GameMem Gamemem_info;

void ResetOffset(Memory * mempool) {
    mempool->offset = 0;
}
