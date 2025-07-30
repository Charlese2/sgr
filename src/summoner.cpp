#include "game/summoner.h"
#include "game/SoundSystem.h"
#include "game/MusicSystem.h"
#include "game/loading.h"
#include "game/text_effects.h"
#include "game/effect_mem.h"
#include "game/gr_font.h"
#include "game/File.h"
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
sound_volume volume;
Mempool test;

#ifdef DEBUG
console_command call_set_sound_volume("sound_volume", "Set the sound volume", CALL, (CommandCallbackInt)set_sound_volume);
console_command call_set_music_volume("music_volume", "Set the music volume", CALL, (CommandCallbackInt)set_music_volume);
console_command call_print_to_tty("tty_print", "Print to the TTY", CALL, (CommandCallbackInt)print_to_tty);
console_command call_print_break_to_tty("tty_break", "Print a break to the TTY", CALL, (CommandCallbackInt)print_break_to_tty);
#endif

void set_sound_volume() {
    if (calling_a_command_function) {
        process_command(16);
        if (command_argument_float < 0.0f || command_argument_float > 1.0f) {
            doing_help_for_comand = 1;
        } else {
            sound_3d::set_sound_volume(0, command_argument_float);
            volume.m_sound_info.m_sound_effect_volume = command_argument_float;
        }
    }
    if (checking_status_for_command) {
        float volume = SoundConvert::GetSoundVolume(0);
        sprintf(string_buffer, "Sound volume is %f\n", volume);
        print_to_console(string_buffer, 0);
    }
    if (doing_help_for_comand) {
        print_to_console("Usage: sound_volume <volume> -- where volume is number from 0.0 to 1.0\n", 0);
    }
}

void set_music_volume() {
    if (calling_a_command_function) {
        process_command(16);
        if (command_argument_float < 0.0f || command_argument_float > 1.0f) {
            doing_help_for_comand = 1;
        } else {
            sound_3d::set_music_volume(command_argument_float);
            volume.m_sound_info.m_music_volume = command_argument_float;
        }
    }
    if (checking_status_for_command) {
        float volume = gMusicSystem.GetMusicVolume() / 255.0f;
        sprintf(string_buffer, "Music volume is %f\n", volume);
        print_to_console(string_buffer, 0);
    }
    if (doing_help_for_comand) {
        print_to_console("Usage: music_volume <volume> -- where volume is number from 0.0 to 1.0\n", 0);
    }
}

void print_to_tty() {
    Flle file;

    if (calling_a_command_function) {
        process_command(2);
        printf("%s\n", next_arg);
    } else if (doing_help_for_comand) {
        print_to_console("Usage: tty_print 'statement, comment, whatever'", 0);
    }
}

void print_break_to_tty() {
    Flle file;

    if (calling_a_command_function) {
        printf("##############################################################################\n", next_arg);
        process_command(3);
        if (arg_type & ARGUMENT_TYPE_UNKNOWN2) {
            printf("%s\n", next_arg);
        }
    } else if (doing_help_for_comand) {
        print_to_console("Usage: tty_break", 0);
    }
}

void show_title_credits() {
    RealtimeTimer titleCreditsTimer;
    titleCreditsTimer.SetTimeout(505);
    int font_height = gr_font::GetFontHeight();
    while (!titleCreditsTimer.elapsed()) {
        DriveStatus(0, 0);
#ifdef DEBUG
gRenderSystem.Setup2DElementDraw();
#else
gRenderSystem.Setup2DElementDraw(false);
#endif
    }
    TimeTick = OSGetTick();
}

void hide_title_credits() {
    RealtimeTimer titleCreditsTimer;

    titleCreditsTimer.SetTimeout(505);
    while (!titleCreditsTimer.elapsed()) {
        DriveStatus(0, 0);
#ifdef DEBUG
        gRenderSystem.Setup2DElementDraw();
#else
        gRenderSystem.Setup2DElementDraw(false);
#endif
    }
    TimeTick = OSGetTick();
}

void render_title_credits() {
    stop_loadscreen();
    DriveStatus(0, 0);
    show_title_credits();
    DriveStatus(0, 0);
    Mempool *cur_mempool = get_current_mempool();
    DEBUGASSERTLINE(641, cur_mempool == NULL);
    set_current_mempool(NULL);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
    DriveStatus(0, 0);
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
    sound_3d::snd_cleanup(1, 1);
    exit(status);
}

void Initialize() {
    Mempool *cur_mempool;
    cur_mempool = get_current_mempool();
    set_current_mempool(NULL);
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
    set_current_mempool(cur_mempool);

    Level_sequence_mempool.Copy((u8 *)Level_sequences, sizeof(level_sequences), "Level sequences", 4);
    Cutscene_sequence_mempool.Copy((u8 *)Cutscene_sequences, sizeof(cutscene_sequences), "Cutscene seqs", 4);
    Quest_mempool.Copy((u8 *)Quests, sizeof(quests), "Quests", 4);
}

void MainLoop() {
    NGCSystem::InitializeSystems();
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
