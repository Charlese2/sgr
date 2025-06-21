#ifndef _GAME_FILESYSTEM_H_
#define _GAME_FILESYSTEM_H_

#include "game/macros.h"
#include "game/CrankyFileManager.h"

extern CrankyFileManager gFileManager;
extern const char PathIds[10][16];

const int kNumPathIds = 10;

void LoadFile(void);
void SetupFilesystem(void);

inline const char *GetFilePath(int index) {
    DEBUGASSERTLINE(57, index >= 0 && index < kNumPathIds);
    return PathIds[index];
}

#endif // _GAME_FILESYSTEM_H_
