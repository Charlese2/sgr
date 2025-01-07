#include "game/debug.h"
#include "dolphin/os.h"
#include <stdio.h>

void DebugError(char * File, u32 Line, char * Error) {
char charBuffer [1052];
    sprintf(charBuffer, "Error: %s\nFile: %s - Line: %d\n\n", Error, File, Line);
    printf(charBuffer);
    OSPanic("debug.cpp", 0x33, charBuffer);
}
