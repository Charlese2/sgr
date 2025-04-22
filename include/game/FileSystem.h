#include "game/macros.h"

typedef const char PathId[16];

const int kNumPathIds = 10;

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

inline PathId* GetFilePath(int index) {
    DEBUGASSERTLINE(57, index >= 0 && index < kNumPathIds);
    return &PathIds[index];
}
