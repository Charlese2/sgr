#include "game/FileSystem.h"
#include "game/gamemem.h"

extern void file_found(void);
extern void file_missing(char* file);

CrankyFileManager gFileManager;

void LoadFileCallback() {
    
}

void SetupFilesystem(void) {

    gHeapAlloc = true;
    
    gHeapAlloc = false;
    gFileManager.SetMissingFileCallback(&file_missing);
}
