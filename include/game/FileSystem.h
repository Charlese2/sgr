#ifndef _GAME_FILESYSTEM_H_
#define _GAME_FILESYSTEM_H_

#include "game/macros.h"
#include "game/CrankyFileManager.h"

extern CrankyFileManager gFileManager;

typedef const char PathId[16];

const int kNumPathIds = 10;

void LoadFile(void);
void SetupFilesystem(void);

PathId PathIds[10] = {
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

inline char* GetFilePath(int index) {
    DEBUGASSERTLINE(57, index >= 0 && index < kNumPathIds);
    return (char*)PathIds[index];
}

#endif // _GAME_FILESYSTEM_H_
