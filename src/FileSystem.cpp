#include "game/FileSystem.h"
#include "game/gamemem.h"

const char PathIds[10][16] = {
    "/",
    "/char/",
    "/fx/",
    "/itm/",
    "/lvl/",
    "/snd/",
    "/fnt/",
    "/mov/",
    "/cuts/",
    "/mus/",
};

extern void file_found(void);
extern void file_missing(char* file);

CrankyFileManager gFileManager;

void LoadFileCallback() {
    
}

void FileSystem::Setup(void) {

    gHeapAlloc = true;
    
    gHeapAlloc = false;
    gFileManager.SetMissingFileCallback(&file_missing);
}
