#include "game/gameplay.h"

GameplayStage Gameplay_stage;

void RunLevel() {
    Gameplay_stage = Running;

    Gameplay_stage = Idle;
}
