#include "game/summoner.h"
#include "dolphin/types.h"
#include "string.h"
#include "game/gamemem.h"
#include <stdlib.h>

extern void StatusStuff(u32, u32, double, double);
extern void SoundStuff();

extern bool gHeapAlloc;

double status1;
double status2;

mainParameters parameters;

void Cleanup(int status) {
    SoundStuff();
    exit(status);
}

void Initialize() {
    Mempool* memPool;
    void* address;
    memPool = get_current_mempool();
    set_current_mempool(0);
    gHeapAlloc = true;

    set_current_mempool(memPool);
}

void MainLoop() 
{

    Initialize();

    Gamemem_info.ActivateGamemem();
    Gamemem_info.ActivatePerlevelMempool();
    Gamemem_info.ActivatePersistantMempool();

    printf("**** Game startup time: %.3f ***");
}

void setParameters() {
    mainParameters* test;
    bool test2;

    memset(&parameters, 0xff, 18);
    parameters.valid_demo = 0;
    test = &parameters;

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


int main()
{
    setParameters();
    MainLoop();
    Cleanup(3);
    return 0;
}
