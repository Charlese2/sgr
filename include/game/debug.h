#include "dolphin.h"


const char format[32] = "Error: %s\nFile: %s - Line: %d\n\n";
const char file[10] = "debug.cpp";

void DebugError(char * File, u32 Line, char * Error);
