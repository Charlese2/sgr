enum GameplayStage {
    Idle,
    Level_Init,
    Running,
    Level_Close,
};

extern GameplayStage Gameplay_stage;

void RenderGameplay(void);
void RunLevel(void);
