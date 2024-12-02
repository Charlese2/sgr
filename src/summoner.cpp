#include "dolphin/types.h"
#include "printf.h"
#include "string.h"

extern void fn_801535b4();
extern void StatusStuff(u32, u32, double, double);
extern int fn_801508F8();
extern void fn_80150900();
extern u32* SomeMemoryFunction(int amount, char * file, int line);
extern u32* fn_801b7a64(u32*, void*, void*, u32, u32);
extern void fn_80014a5c();
extern void* fn_800baf7c(void*);
extern void* fn_800baf10(void*, void*);

extern volatile BOOL AllocatingMemory;
extern u32* lbl_80474f74;
extern u32* lbl_80474b70;
extern u32* lbl_804743f0;
extern u32* lbl_80475640;
extern u32* lbl_8047545c;
extern u32* lbl_80475464;
extern u32* lbl_80474ef8;
extern u32* lbl_80474b7c;
extern u32* lbl_80474b58;
extern u32* lbl_80474b4c;
extern u32* lbl_80473e40;
extern u32* lbl_80473e44;
extern u32* lbl_804745e8;

double status1;
double status2;
static void MainLoop() 
{
    u32* allocated_memory_address;

    fn_801535b4();
    StatusStuff(0,0, status1, status2);

    int someValue = fn_801508F8();
    fn_80150900();
    AllocatingMemory = TRUE;
    allocated_memory_address = SomeMemoryFunction(0x9c50U, "summoner.cpp", 796);
    lbl_80474f74 = fn_801b7a64(allocated_memory_address, (void*)fn_80014a5c, NULL, 40, 1000);
    lbl_80474b70 = SomeMemoryFunction(0x27f28U, "summoner.cpp", 797);
    lbl_804743f0 = SomeMemoryFunction(0x32000U, "summoner.cpp", 798);
    lbl_80475640 = SomeMemoryFunction(0x20000U, "summoner.cpp", 799);
    lbl_8047545c = SomeMemoryFunction(0x32000U, "summoner.cpp", 800);
    lbl_80475464 = SomeMemoryFunction(0x2000U, "summoner.cpp", 801);
    allocated_memory_address = SomeMemoryFunction(0x42730U, "summoner.cpp", 802);
    lbl_80474ef8 = fn_801b7a64(allocated_memory_address, (void*)fn_800baf7c, (void*)fn_800baf10, 0x5e8, 180);
    lbl_80474b7c = SomeMemoryFunction(0x45d30U, "summoner.cpp", 803);
    lbl_80474b58 = SomeMemoryFunction(0x1fa18U, "summoner.cpp", 805);
    lbl_80474b4c = SomeMemoryFunction(0x1d800U, "summoner.cpp", 804);
    lbl_80473e40 = SomeMemoryFunction(0x23280U, "summoner.cpp", 806);
    lbl_80473e44 = SomeMemoryFunction(0x23280U, "summoner.cpp", 807);
    allocated_memory_address = SomeMemoryFunction(0x32000U, "summoner.cpp", 808);
    lbl_804745e8 = fn_801b7a64(allocated_memory_address, (void*)fn_80014a5c, NULL, 40, 1000);

    
    do {
        
    } while (true);
}

short main_parameters[10];

/* 80014180-80014344       .text main */
void main()
{
    memset(&main_parameters, 0xff, 18);
    main_parameters[8] = (short)0;

    int remaining = 2;
    do {
        if (main_parameters[0] != 0xff) {
            main_parameters[8] = 1;
            break;
        }
        if (main_parameters[0] != 0xff) {
            main_parameters[8] = 1;
            break;
        }
        if (main_parameters[0] != 0xff) {
            main_parameters[8] = 1;
            break;
        }
        if (main_parameters[0] != 0xff) {
            main_parameters[8] = 1;
            break;
        }
        if (main_parameters[0] != 0xff) {
            main_parameters[8] = 1;
            break;
        }
        if (main_parameters[0] != 0xff) {
            main_parameters[8] = 1;
            break;
        }

        remaining--;
    } while (remaining != 0);



    if (main_parameters[8] == 0) {
        printf("Not a valid demo.  Setting default parameters.\n");
        main_parameters[0] = 0;
        main_parameters[1] = 1;
        main_parameters[2] = 2;
        main_parameters[5] = 6;
        main_parameters[3] = 0;
        main_parameters[4] = 6;
        main_parameters[6] = 10;
        main_parameters[7] = 0;
    } else {
        printf("Using SCE demo parameters.\n");
    }

    MainLoop();
}
