#include "game/summoner.h"
#include "game/timestamp.h"
#include "game/characterinfo.h"
#include "game/InputSystem.h"
#include "game/quests.h"
#include "game/level_sequences.h"
#include "game/cutscene_sequences.h"
#include "game/items.h"
#include "game/inv.h"
#include "game/vif_particles.h"
#include "game/gamemem.h"
#include "game/NGCSystem.h"
#include "game/living_entity.h"
#include "game/gr.h"
#include "dolphin/types.h"
#include "dolphin/os.h"
#include <stdlib.h>

extern void StatusStuff(u32, u32, double, double);
extern void SoundStuff();

extern bool gHeapAlloc;

double status1;
double status2;

OSTick TimeTick;

extern pathfinding_transform *PathfindingTransforms;
extern living_entity *Living_entities;
extern effect_transform *EffectTransforms;
extern characterinfo *Character_info;
statscreen *Stat_screen;
extern quests *Quests;
extern level_sequences *Level_sequences;
extern cutscene_sequences *Cutscene_sequences;
extern characterinfo_something *Character_info_unk;
extern items *Items;
extern inv *Inventory;
extern vif_particles *Vif_particles[2];
extern Mempool Level_sequence_mempool;
extern Mempool Cutscene_sequence_mempool;
extern Mempool Quest_mempool;

mainParameters parameters;

void show_title_credits() {
    RealtimeTimer titleCreditsTimer;
    titleCreditsTimer.SetTimeout(505);
    while (!titleCreditsTimer.elapsed()) {
        DriveStatus(0, 0);
        gRenderSystem.Setup2DElementDraw();
    }
    TimeTick = OSGetTick();
}

void hide_title_credits() {
    RealtimeTimer titleCreditsTimer;

    titleCreditsTimer.SetTimeout(505);
    while (!titleCreditsTimer.elapsed()) {
        DriveStatus(0, 0);
        gRenderSystem.Setup2DElementDraw();
    }
    TimeTick = OSGetTick();
}

void InitializeInput() {
#ifdef DEBUG
    gInputSystem.Initialize(1);
#else
    gInputSystem.Initialize(0);
#endif
}

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
    Level_sequences       = (level_sequences *)operator new[](sizeof(level_sequences), __FILE__, 800);
    Cutscene_sequences    = (cutscene_sequences *)operator new[](sizeof(cutscene_sequences), __FILE__, 801);
    Living_entities       = NEW_ALLOCATION(802, living_entity[180]);
    Character_info_unk    = (characterinfo_something *)operator new[](sizeof(characterinfo_something), __FILE__, 803);
    Items                 = (items *)operator new[](sizeof(items), __FILE__, 804);
    Inventory             = (inv *)operator new[](sizeof(inv), __FILE__, 805);
    Vif_particles[0]      = (vif_particles *)operator new[](sizeof(vif_particles), __FILE__, 806);
    Vif_particles[1]      = (vif_particles *)operator new[](sizeof(vif_particles), __FILE__, 807);
    EffectTransforms      = NEW_ALLOCATION(808, effect_transform[1000]);

    gHeapAlloc = false;
    set_current_mempool(memPool);

    Level_sequence_mempool.Copy((u8 *)Level_sequences, sizeof(level_sequences), "Level sequences", 4);
    Cutscene_sequence_mempool.Copy((u8 *)Cutscene_sequences, sizeof(cutscene_sequences), "Cutscene seqs", 4);
    Quest_mempool.Copy((u8 *)Quests, sizeof(quests), "Quests", 4);
}

void MainLoop() {
    InitializeNGCSystems();
    DriveStatus(0, 0);
    Initialize();
    DriveStatus(0, 0);

    Gamemem_info.ActivateGamemem();
    DriveStatus(0, 0);
    Gamemem_info.ActivatePerlevelMempool();
    Gamemem_info.ActivatePersistantMempool();
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    GraphicsInit(512, 448, 105, 201, 0, 1);
    DriveStatus(0, 0);

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
