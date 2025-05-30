#include "game/summoner.h"
#include "game/characterinfo.h"
#include "game/level_sequences.h"
#include "game/quests.h"
#include "game/gamemem.h"
#include "game/living_entity.h"
#include "dolphin/types.h"
#include <stdlib.h>

extern void StatusStuff(u32, u32, double, double);
extern void SoundStuff();

extern bool gHeapAlloc;

double status1;
double status2;

extern pathfinding_transform *PathfindingTransforms;
extern living_entity *living_entities;
extern effect_transform *EffectTransforms;
extern characterinfo *Character_info;
statscreen *Stat_screen;
extern quests *Quests;
extern level_sequences_loadsave *Level_sequences;

mainParameters parameters;

void Cleanup(int status) {
    SoundStuff();
    exit(status);
}

void Initialize() {
    Mempool *memPool;
    memPool = get_current_mempool();
    set_current_mempool(0);
    gHeapAlloc = true;

    PathfindingTransforms = NEW_ALLOCATION(796, pathfinding_transform[1000]);
    Character_info        = (characterinfo *)operator new[](sizeof(characterinfo), __FILE__, 797);
    Stat_screen           = (statscreen *)operator new[](sizeof(statscreen), __FILE__, 798);
    Quests                = (quests *)operator new[](sizeof(quests), __FILE__, 799);
    Level_sequences       = (level_sequences_loadsave*)operator new[](sizeof(level_sequences_loadsave), __FILE__, 800);
    living_entities       = NEW_ALLOCATION(802, living_entity[180]);
    EffectTransforms      = NEW_ALLOCATION(808, effect_transform[1000]);

    set_current_mempool(memPool);
}

void MainLoop() {

    Initialize();

    Gamemem_info.ActivateGamemem();
    Gamemem_info.ActivatePerlevelMempool();
    Gamemem_info.ActivatePersistantMempool();

    printf("**** Game startup time: %.3f ***");
}

void setParameters(int input1, char *argv[]) {
    mainParameters *test;
    bool test2;

    memset(&parameters, 0xff, 18);
    parameters.valid_demo = 0;
    test                  = &parameters;

    for (int i = 0; i < 18; i++) {
    }

    if (parameters.valid_demo == 0) {
        printf("Not a valid demo.  Setting default parameters.\n");
        parameters.unk0 = 0;
        parameters.unk2 = 1;
        parameters.unk4 = 2;
        parameters.unkA = 6;
        parameters.unk6 = 0;
        parameters.unk8 = 0;
        parameters.unkC = 10;
        parameters.unkE = 0;
    } else {
        printf("Using SCE demo parameters.\n");
    }

    test2 = parameters.unk0;
}

int main(int argc, char *argv[]) {
    setParameters(argc, argv);
    MainLoop();
    Cleanup(3);
    return 0;
}
