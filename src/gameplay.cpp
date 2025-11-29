#include "game/gameplay.h"
#include "game/gamewide.h"
#include "game/frametime.h"
#include "game/interface.h"
#include "game/delay.h"

GameplayStage Gameplay_stage;

void RenderGameplay() {
    if ((Game_mode & 0x1000) == 0) {
        
    }
}

void RunLevel() {
    Gameplay_stage = Running;
    if (Game_mode & 0x80000000) {
        increase_frametime_pause_counter();
        Wait(10);
        increase_frametime_pause_counter();
    }
    Time_played += reported_frametime;

    Gameplay_stage = Idle;
}
