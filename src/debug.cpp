#include "game/debug.h"
#include <stdio.h>

void DebugError(char * File, u32 Line, char * Error) {
char charBuffer [1052];
    sprintf(charBuffer, format, Error, File, Line);
    printf(charBuffer);
    OSPanic(file, 0x33, charBuffer);
}
