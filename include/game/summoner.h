#include "dolphin/types.h"

void set_sound_volume();
void set_music_volume();
void print_to_tty();
void print_break_to_tty();
void show_title_credits();
void hide_title_credits();
void render_title_credits();

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
    u16 unk6;
    u16 unk8;
    u16 unkA;
    u16 unkC;
    u16 unkE;
    u16 valid_demo;
} mainParameters;
