
#include "game/sound_3d.h"
#include "game/gameplay.h"

void ReleaseFromMark() { sound_3d::snd_release(); }

void LevelInit() {
    Gameplay_stage = Level_Init;

    Gameplay_stage = Idle;
}
