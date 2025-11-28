#include "game/summoner.h"
#include "game/SoundSystem.h"
#include "game/MusicSystem.h"
#include "game/FileSystem.h"
#include "game/PackFileSystem.h"
#include "game/interface.h"
#include "game/gamewide.h"
#include "game/loading.h"
#include "game/frametime.h"
#include "game/memory.h"
#include "game/mainloop.h"
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
#include "game/console.h"
#include "dolphin/types.h"
#include "dolphin/os.h"
#include <stdlib.h>

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

int file_id = -1;

mainParameters parameters;
sound_volume volume;
Mempool test;
Color background_color(150, 166, 212, 255);
char fx_mem_info_initialized;
int startup_time_start;
int startup_time_end;

int font_number;

const char *credits[8] = {
#ifdef DEBUG
    "Summoner \xaf: The Prophecy - \xaa 2002 THQ Inc.",     "Developed by Cranky Pants Games and Volition Inc.",
    "Uses Bink Video. Copyright \xaa 1991-2002 by RAD",     "Game Tools, Inc. Summoner, Cranky Pants Games,",
    "Volition, THQ and their respective logos are",         "trademarks and/or registered trademarks of THQ Inc.",
    "All rights reserved. All other trademarks, logos and", "copyrights are property of their respective owners.",
#else
    "Summoner \xaf: A Goddess Reborn - \xaa 2002, 2003",       "THQ Inc. Developed by Cranky Pants Games and Volition,",
    "Inc. Uses Bink Video. Copyright \xaa 1991-2002 by RAD",   "Game Tools, Inc. Summoner, Cranky Pants Games,",
    "Volition, THQ and their respective logos are trademarks", "and/or registered trademarks of THQ Inc. All rights",
    "reserved. All other trademarks, logos and copyrights",    "are property of their respective owners.",

#endif
};

#ifdef DEBUG
console_command call_set_sound_volume("sound_volume", "Set the sound volume", CALL, (CommandCallbackInt)set_sound_volume);
console_command call_set_music_volume("music_volume", "Set the music volume", CALL, (CommandCallbackInt)set_music_volume);
console_command call_print_to_tty("tty_print", "Print to the TTY", CALL, (CommandCallbackInt)print_to_tty);
console_command call_print_break_to_tty("tty_break", "Print a break to the TTY", CALL, (CommandCallbackInt)print_break_to_tty);
#endif

effects_mem_info fx_mem_info;

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
    File file;

    if (calling_a_command_function) {
        process_command(2);
        printf("%s\n", next_arg);
    } else if (doing_help_for_comand) {
        print_to_console("Usage: tty_print 'statement, comment, whatever'", 0);
    }
}

void print_break_to_tty() {
    File file;

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
    int font_height = gr_font::GetFontHeight(font_number);
    int width       = 0;
    int height;
    int credits_height;
    int credits_width;
    int i;
    for (i = 0; i < 8; i++) {
        gr_font::setup_text_effect(&credits_height, &credits_width, credits[i], -1, -1);
        if (credits_width > width) {
            width = credits_width;
        }
    }

    int credits2_height = 0;
    int credits2_width  = 0;
    gr_font::setup_text_effect(&credits2_height, &credits2_width, "Licensed by Nintendo", -1, -1);
    credits2_height = (512 - credits2_height) / 2;
    credits2_width  = 428 - credits2_width;

    titleCreditsTimer.SetTimeout(505);
    while (!titleCreditsTimer.elapsed()) {
        NGCSystem::DriveStatus(0, 0);
        gRenderSystem.StartDraw();
#ifdef DEBUG
        gRenderSystem.StartDraw2D();
#else
        gRenderSystem.StartDraw2D(false);
#endif
        gr::DrawHudColor(background_color);
        for (i = 0; i < 8; i++) {
            gr_font::DrawTextOnScreen2D((512 - width) / 2, (font_height * -8 + 448), credits[i], -1);
        }
        gr_font::DrawTextOnScreen2D(credits2_height, credits2_width, "Licensed by Nintendo", -1);
        FrameDone();
    }
    TimeTick = OSGetTick();
}

void hide_title_credits() {
    RealtimeTimer titleCreditsTimer;
    int font_height = gr_font::GetFontHeight(font_number);
    int width       = 0;
    int height;
    int credits_height;
    int credits_width;
    int i;
    for (i = 0; i < 8; i++) {
        gr_font::setup_text_effect(&credits_height, &credits_width, credits[i], -1, -1);
        if (credits_width > width) {
            width = credits_width;
        }
    }

    titleCreditsTimer.SetTimeout(505);
    while (!titleCreditsTimer.elapsed()) {
        NGCSystem::DriveStatus(0, 0);
#ifdef DEBUG
        gRenderSystem.StartDraw2D();
#else
        gRenderSystem.StartDraw2D(false);
#endif
        gr::DrawHudColor(background_color);
        for (i = 0; i < 8; i++) {
            gr_font::DrawTextOnScreen2D((512 - width) / 2, (font_height * -8 + 448), credits[i], -1);
        }
        FrameDone();
    }
    TimeTick = OSGetTick();
}

void init_graphics() { gr::init(512, 448, 105, 201, 0, 1); }

void startup() {
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    startup_time_start = NGCSystem::GetTimeFromTicks(1000);
    NGCSystem::DriveStatus(0, 0);
    init_graphics();
    NGCSystem::DriveStatus(0, 0);
    set_current_mempool(&Gamemem_info.GetGameMem()->persistantMempool);
    gPackFileSystem.add_pack_file("fonts.pkf", 1, 32, 6);
    NGCSystem::DriveStatus(0, 0);
    gPackFileSystem.add_pack_file("persistent.pkf", 1, 0, 0);
    NGCSystem::DriveStatus(0, 0);
    set_current_mempool(NULL);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
}

void render_title_credits() {
    loading::stop_loadscreen();
    NGCSystem::DriveStatus(0, 0);
    show_title_credits();
    NGCSystem::DriveStatus(0, 0);
    Mempool *cur_mempool = get_current_mempool();
    DEBUGASSERTLINE(641, cur_mempool == NULL);
    set_current_mempool(NULL);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    hide_title_credits();
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    ResetMainMenuIndexes();
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
    NGCSystem::DriveStatus(0, 0);
    Initialize();
    NGCSystem::DriveStatus(0, 0);
    if (!fx_mem_info_initialized) {
        fx_mem_info.initialize();
        fx_mem_info_initialized = true;
    }
    fx_mem_info.initialize_info();
    DEBUGASSERTLINE(817, fx_mem_info.verify());
    Fx_mem_external_info = &fx_mem_info;
    SetupFileSystem();
    NGCSystem::DriveStatus(0, 0);
    GameMem::ActivateGamemem();
    NGCSystem::DriveStatus(0, 0);
    Gamemem_info.ActivatePerlevelMempool();
    Gamemem_info.ActivatePersistantMempool();
    startup();
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    NGCSystem::DriveStatus(0, 0);
    if (parameters.valid_demo) {
        for (int i = 0; i < 4; i++) {
            volumeInfo.volumes[i] = parameters.unkC / 10.0f;
        }
    }
    render_title_credits();
    NGCSystem::DriveStatus(0, 0);
    if (!parameters.unk4) {
        int status = PlayVideo("ATTRACT.PSS", 1);
        Cleanup(status != 0);
        Gamemem_info.ClearPerlevelMempool();
    } else {
        PlayVideo("LOGOS.PSS", 1);
    }
    NGCSystem::DriveStatus(0, 0);
    set_current_mempool(Gamemem_info.GetPerlevelMempool());
    file_id = gPackFileSystem.add_pack_file("mainmenu.pkf", 0, 0, 0);
    NGCSystem::DriveStatus(0, 0);
    front_end_peg_loaded = true;
    set_current_mempool(NULL);
    startup_time_end = NGCSystem::GetTimeFromTicks(1000);
    printf("**** Game startup time: %.3f ***", (startup_time_end - startup_time_start) * 0.001f);
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
