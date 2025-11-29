#ifndef _GAME_FILESYSTEM_H_
#define _GAME_FILESYSTEM_H_

#include "game/macros.h"
#include "game/CrankyFileManager.h"

#define FILE_MAX_BASE_FILENAME_LEN 80
#define MAX_PATH_LENGTH 16

extern CrankyFileManager gFileManager;
const int kNumPathIds = 10;

extern const char PathIds[kNumPathIds][MAX_PATH_LENGTH];


void LoadFile(void);
void SetupFileSystem(void);
bool PkfFileTypeHandler(int *unk);

inline const char *GetFilePath(int index) {
    DEBUGASSERTLINE(57, index >= 0 && index < kNumPathIds);
    return PathIds[index];
}

#endif // _GAME_FILESYSTEM_H_
