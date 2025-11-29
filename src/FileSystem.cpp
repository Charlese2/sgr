#include "game/FileSystem.h"
#include "game/gamemem.h"

const char PathIds[kNumPathIds][MAX_PATH_LENGTH] = {
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

void SetupFileSystem(void) {

    gHeapAlloc = true;
    
    gHeapAlloc = false;
    gFileManager.SetMissingFileCallback(&file_missing);
}
