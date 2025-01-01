#include "dolphin/types.h"
#include "string.h"
#include "game/memory.h"

void MainLoop();

extern void StatusStuff(u32, u32, double, double);
extern void SoundStuff();

extern volatile BOOL AllocatingMemory;
extern void* lbl_80474f74;
extern void* lbl_80474b70;
extern void* lbl_804743f0;
extern void* lbl_80475640;
extern void* lbl_8047545c;
extern void* lbl_80475464;
extern void* lbl_80474ef8;
extern void* lbl_80474b7c;
extern void* lbl_80474b58;
extern void* lbl_80474b4c;
extern void* lbl_80473e40;
extern void* lbl_80473e44;
extern void* lbl_804745e8;

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
}

void MainLoop() 
{
    void * allocated_memory_address;

    StatusStuff(0,0, status1, status2);

    AllocatingMemory = TRUE;

    AllocatingMemory = FALSE;


    
    do {
        
    } while (true);
}
