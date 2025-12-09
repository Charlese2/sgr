#include "game/gameplay.h"
#include "dolphin/pad.h"
#include "game/gamewide.h"
#include "game/InputSystem.h"
#include "game/fullscreen.h"
#include "game/objectrender.h"
#include "game/player.h"
#include "game/gr.h"
#include "game/gr_font.h"
#include "game/frametime.h"
#include "game/interface.h"
#include "game/delay.h"
#include "game/vector.h"
#include <stdio.h>

GameplayStage Gameplay_stage;
bool draw_position_text;
bool unk;

void RenderGameplay() {
    if ((Game_mode & 0x1000) == 0) {
        gr::DrawLoadingScreenBackground();
    }
    if (unk) {
        Game_mode & 0x10; //BUG: typo
    }
    if (!new_fullscreen_in_fullscreen_mode() && get_interface_to_open() != worldmap ){
        Render3DObjects();
    }
#ifdef DEBUG
    gRenderSystem.StartDraw2D();
    if (Main_entity) {
        if (gInputSystem.is_button_just_pressed(1, PAD_BUTTON_X, true)) {
            draw_position_text = draw_position_text ^ true;
        }
        if (draw_position_text) {
            vector3 position;
            vector3x3 test;
            char buffer[32];
            sprintf(buffer, "pos: <%5.1f,%5.1f,%5.1f>", position.x, position.y, position.z);
            gr_font::DrawTextOnScreen2D(300, 10, buffer, font_number);
        }
    }
    gRenderSystem.EndDraw2D();
#endif
}

void RunLevel() {
    Gameplay_stage = Running;
    if (Game_mode & 0x80000000) {
        increase_frametime_pause_counter();
        Wait(10);
        increase_frametime_pause_counter();
    }
    Time_played += reported_frametime;

    if (Game_mode & 0x200) {
        RenderGameplay();
    }

    Gameplay_stage = Idle;
}
