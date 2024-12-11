#include "dolphin/types.h"
#include "string.h"

void MainLoop();

extern void fn_801535b4();
extern void StatusStuff(u32, u32, double, double);
extern int fn_801508F8();
extern void fn_80150900();
extern u32* SomeMemoryFunction(int amount, const char * file, int line);
extern u32* fn_801b7a64(u32*, void*, void*, u32, u32);
extern void fn_80014a5c();
extern void* fn_800baf7c(void*);
extern void* fn_800baf10(void*, void*);
extern void SoundStuff();
extern void fn_80180e38(BOOL, BOOL);
extern void fn_801848c4();
extern void fn_80183350();
extern void fn_80182e70();

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

const char attract_pss_string[12] = "ATTRACT.PSS";
const char cutscene_string[9] = "cutscene";
const char character_string[10] = "character";
const char levelchar_string[10] = "levelchar";
const char title_string[44] = "Summoner \xaf: A Goddess Reborn - \xaa 2002, 2003";
const char title_credits1_string[55] = "THQ Inc. Developed by Cranky Pants Games and Volition,";
const char title_credits2_string[52] = "Inc. Uses Bink Video. Copyright \xaa 1991-2002 by RAD";
const char title_credits3_string[47] = "Game Tools, Inc. Summoner, Cranky Pants Games,";
const char title_credits4_string[56] = "Volition, THQ and their respective logos are trademarks";
const char title_credits5_string[52] = "and/or registered trademarks of THQ Inc. All rights";
const char title_credits6_string[53] = "reserved. All other trademarks, logos and copyrights";
const char title_credits7_string[41] = "are property of their respective owners.";
const char title_credits8_string[10] = "LOGOS.PSS";
const char using_sce_demo_string[28] =  "Using SCE demo parameters.\n";
const char not_valid_demo_string[48] = "Not a valid demo.  Setting default parameters.\n";
const char file_string[13] = "summoner.cpp";
const char level_sequences_string[16] = "Level sequences";
const char Cutscene_seqs_string[14] = "Cutscene seqs";
const char fonts_pkf_string[10] = "fonts.pkf";
const char persistent_pkf_string[15] = "persistent.pkf";
const char s2_larget_vfn_string[13] = "s2-large.vfn";
const char s2_small_vfn_string[13] = "s2-small.vfn";
const char mainmenu_pkf_string[13] = "mainmenu.pkf";
const char game_startup_time_string[36] = "**** Game startup time: %.3f ****\n";

short main_parameters[10];

void main()
{
    int remaining = (short)2;

    memset(&main_parameters, 0xff, 18);
    main_parameters[8] = (short)0;

    
    do {
        if (main_parameters[0] != 0xff) {
            main_parameters[8] = 1;
            break;
        }
        if (main_parameters[1] != 0xff) {
            main_parameters[8] = 1;
            break;
        }
        if (main_parameters[3] != 0xff) {
            main_parameters[8] = 1;
            break;
        }
        if (main_parameters[2] != 0xff) {
            main_parameters[8] = 1;
            break;
        }
        if (main_parameters[5] != 0xff) {
            main_parameters[8] = 1;
            break;
        }
        if (main_parameters[3] != 0xff) {
            main_parameters[8] = 1;
            break;
        }
        if (main_parameters[7] != 0xff) {
            main_parameters[8] = 1;
            break;
        }
        if (main_parameters[9] != 0xff) {
            main_parameters[8] = 1;
            break;
        }

        remaining = remaining + -1;
    } while (remaining != 0);



    if (main_parameters[8] == 0) {
        printf(not_valid_demo_string);
        main_parameters[0] = 0;
        main_parameters[1] = 1;
        main_parameters[2] = 2;
        main_parameters[5] = 6;
        main_parameters[3] = 0;
        main_parameters[4] = 6;
        main_parameters[6] = 10;
        main_parameters[7] = 0;
    } else {
        printf(using_sce_demo_string);
    }

    MainLoop();
    SoundStuff();
    fn_80180e38(TRUE,TRUE);
    fn_801848c4();
    fn_80183350();
    fn_80182e70();
}

void MainLoop() 
{
    u32* allocated_memory_address;

    fn_801535b4();
    StatusStuff(0,0, status1, status2);

    int someValue = fn_801508F8();
    fn_80150900();
    AllocatingMemory = TRUE;
    allocated_memory_address = SomeMemoryFunction(0x9c50U, file_string, 796);
    lbl_80474f74 = fn_801b7a64(allocated_memory_address, (void*)fn_80014a5c, NULL, 40, 1000);
    lbl_80474b70 = SomeMemoryFunction(0x27f28U, file_string, 797);
    lbl_804743f0 = SomeMemoryFunction(0x32000U, file_string, 798);
    lbl_80475640 = SomeMemoryFunction(0x20000U, file_string, 799);
    lbl_8047545c = SomeMemoryFunction(0x32000U, file_string, 800);
    lbl_80475464 = SomeMemoryFunction(0x2000U, file_string, 801);
    allocated_memory_address = SomeMemoryFunction(0x42730U, file_string, 802);
    lbl_80474ef8 = fn_801b7a64(allocated_memory_address, (void*)fn_800baf7c, (void*)fn_800baf10, 0x5e8, 180);
    lbl_80474b7c = SomeMemoryFunction(0x45d30U, file_string, 803);
    lbl_80474b58 = SomeMemoryFunction(0x1fa18U, file_string, 805);
    lbl_80474b4c = SomeMemoryFunction(0x1d800U, file_string, 804);
    lbl_80473e40 = SomeMemoryFunction(0x23280U, file_string, 806);
    lbl_80473e44 = SomeMemoryFunction(0x23280U, file_string, 807);
    allocated_memory_address = SomeMemoryFunction(0x32000U, file_string, 808);
    lbl_804745e8 = fn_801b7a64(allocated_memory_address, (void*)fn_80014a5c, NULL, 40, 1000);
    AllocatingMemory = FALSE;


    
    do {
        
    } while (true);
}
