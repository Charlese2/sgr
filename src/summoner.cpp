#include "game/summoner.h"
#include "game/gamemem.h"
#include "game/living_entity.h"
#include "dolphin/types.h"
#include "string.h"
#include <stdlib.h>

extern void StatusStuff(u32, u32, double, double);
extern void SoundStuff();

extern bool gHeapAlloc;

double status1;
double status2;

living_entity *living_entities;
transform *transforms;
void *test;
void *test2;
void *test3;
void *test4;
void *test5;
void *test6;
void *test7;
void *test8;
void *test9;
void *test10;
void *test11;
void *test12;
void *test13;

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

    transforms = new transform[1000];
    test       = NEW_ALLOCATION(0x9c50, 767);
    test2      = NEW_ALLOCATION(0x27f28, 768);
    test3      = NEW_ALLOCATION(0x32000, 769);
    test4      = NEW_ALLOCATION(0x20000, 770);
    test5      = NEW_ALLOCATION(0x32000, 771);
    test6      = NEW_ALLOCATION(0x2000, 772);

        test8 = NEW_ALLOCATION(0x45d30, 774);

    living_entities = new living_entity[180]();

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
