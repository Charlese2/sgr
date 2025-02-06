#include "game/debug.h"
#include "game/gr.h"
#include "dolphin/os.h"
#include <stdio.h>

void DebugPrint(char * buffer) {
    if (getGrInitialized()) {
        printf(buffer);
    }
    OSPanic("debug.cpp", 51, buffer);
}

void DebugError(char* File, u32 Line, char* Error) {
    char charBuffer [1052];
    sprintf(charBuffer, "Error: %s\nFile: %s - Line: %d\n\n", Error, File, Line);
    DebugPrint(charBuffer);
}

int DebugAssert(char* File, u32 Line, char*) {
    
}
