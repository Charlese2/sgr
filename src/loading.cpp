#include "game/loading.h"
#include <stdio.h>
#include "dolphin/vi.h"
#include "game/levels.h"

bool loadscreen_started;

void loading::start_loadscreen() {}

void loading::stop_loadscreen() {
    if (loadscreen_started) {
        levels::SetFogData(NULL, false);
        printf("stop_loadscreen()");
        loadscreen_started = false;
        VISetPostRetraceCallback(NULL);
    }
}

void loading::pause_loadscreen() {}

void loading::unpause_loadscreen() {}
