#include "game/debug.h"
#include "game/loading.h"
#include "game/gr.h"
#include "game/gr_ngc.h"
#include "dolphin/os.h"
#include <stdio.h>

void DebugPrint(char* buffer) {
    if (getGrInitialized()) {
        stop_loadscreen();
        is_current_mode(&gRenderSystem, 1);
        printf(buffer);
        set_alpha_blending_mode(2);
    }
    OSPanic("debug.cpp", 51, buffer);
}

int DebugAssert(char* File, u32 Line, char* Assert) {
    char buffer [1036];

    sprintf(buffer,"Assert: %s\nFile: %s - Line: %d\n\n", Assert, File, Line);
    DebugPrint(buffer);
    return 0;
}

int DebugError(char* File, u32 Line, char* Error) {
    char charBuffer [1052];
    sprintf(charBuffer, "Error: %s\nFile: %s - Line: %d\n\n", Error, File, Line);
    DebugPrint(charBuffer);
    return 0;
}
