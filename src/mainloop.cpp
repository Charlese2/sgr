#include "game/mouse.h"
#include "game/RenderSystem.h"
#include "game/InputSystem.h"
#include "game/mainmenu.h"
#include "game/gameload.h"
#include "game/gameplay.h"

typedef void(MainCallback)();

bool MainLoop_Index_Changed;
int MainMenuIndexCurrent  = -1;
int MainMenuIndexPrevious = -1;
int MainMenuIndexNext     = -1;

MainCallback *mainloop_callbacks[6][3] = { 0, RunMainMenu, 0, 0, 0, 0, NULL, NULL, NULL, LevelInit, RunLevel, LoadLevel, 0, 0, 0, 0, 0, 0 };

void MainMenuIndexProceed() {
    MainMenuIndexPrevious  = MainMenuIndexCurrent;
    MainMenuIndexCurrent   = MainMenuIndexNext;
    MainLoop_Index_Changed = false;
}

void ResetMainMenuIndexes() {
    MainMenuIndexCurrent  = -1;
    MainMenuIndexPrevious = MainMenuIndexCurrent;
    MainMenuIndexNext     = MainMenuIndexCurrent;
}

void SetMainMenuIndexNext(int index) {
    MainMenuIndexNext      = index;
    MainLoop_Index_Changed = true;
}

int GetMainMenuIndexCurrent() { return MainMenuIndexCurrent; }

int GetMainMenuIndexPrevious() { return MainMenuIndexPrevious; }

int GetMainMenuIndexNext() { return MainMenuIndexNext; }

int MainLoopCalls() {
    MainCallback *callback;
    gInputSystem.RunSystem(true);
    while (MainLoop_Index_Changed) {
        if (MainMenuIndexCurrent != -1) {
            callback = mainloop_callbacks[MainMenuIndexCurrent][2];
            if (callback) {
                callback();
            }
        }
        MainMenuIndexProceed();
        callback = mainloop_callbacks[MainMenuIndexCurrent][0];
        if (callback) {
            callback();
        }
    }
    MoveMouse();
    gRenderSystem.StartDraw();
    callback = mainloop_callbacks[MainMenuIndexCurrent][1];
    if (callback) {
        callback();
    }
    return MainMenuIndexCurrent;
}
