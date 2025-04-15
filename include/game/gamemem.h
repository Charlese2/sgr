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

    GameMem();

    static bool ToggleShowGameMemPrints(void);
    static bool ToggleShowMempoolUsage(void);
    int ActivateGamemem(void);
    void AllocateMempools(void);
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

u32 GetHeapHandle(MemSystem * memSystem);

extern GameMem Gamemem_info;

void ResetOffset(Mempool * mempool) {
    mempool->offset = 0;
}
