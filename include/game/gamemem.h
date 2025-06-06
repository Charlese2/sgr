#include "game/console.h"
#include "game/memory.h"
#include "game/MemSystem.h"

extern bool gHeapAlloc;

class GameMem {
  public:
    GameMem();
    static int toggle_show_gamemem_prints(void);
    static int toggle_show_mempool_usage(void);
    int ActivateGamemem(void);
    bool AllocateMempools(void);
    void ActivateSummonMempool(void);
    void DeactivateSummonMempool(void);
    bool summon_mempool_is_active(void);
    Mempool *GetSummonMempool(void);
    void ActivateCutsceneMempool(void);
    void DeactivateCutsceneMempool(void);
    bool cutscene_mempool_is_active(void);
    Mempool *GetCutsceneMempool(void);
    void ActivateSpellslotMempool(void);
    void DeactivateSpellslotMempool(void);
    bool spellslot_mempool_is_active(void);
    Mempool *GetSpellslotMempool(void);
    void ActivatePerlevelMempool(void);
    void DeactivatePerlevelMempool(void);
    bool perlevel_mempool_is_active(void);
    Mempool *GetPerlevelMempool(void);
    void ClearPerlevelMempool(void);
    void ActivatePersistantMempool(void);
    GameMem *GetGameMem(void);
    Mempool *GetSoundMempool(void);
    void DisplayMempoolUsage();

  private:
    Mempool persistantMempool;
    u8 *persistant_data;
    Mempool soundMempool;
    u8 *sound_data;
    Mempool perlevelMempool;
    u8 *perlevel_data;
    Mempool cutsceneMempool;
    u8 *cutscene_data;
    Mempool spellslotMempool;
    u8 *spellslot_data;
    Mempool summonMempool;
    u8 *summon_data;
    bool perlevel_mempool_active;
    bool cutscene_mempool_active;
    bool summon_mempool_active;
    bool spellslot_mempool_active;
    bool persistant_mempool_active;
};

extern GameMem Gamemem_info;
