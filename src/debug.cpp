#include "dolphin.h"
#include "dolphin/types.h"
#include <stdio.h>

const char format[32] = "Error: %s\nFile: %s - Line: %d\n\n";
const char file[10] = "debug.cpp";

void DebugError(char * File, u32 Line, char * Error) {
char charBuffer [1052];
    sprintf(charBuffer, format, Error, File, Line);
    printf(charBuffer);
    OSPanic(file, 0x33, charBuffer);
}
