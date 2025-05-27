#include "global.h"
#include "game/CrankyFileManager.h"

class PackFileSystem {
    virtual ~PackFileSystem();
    int unk4;
    int unk8[61];

    
    public:
    PackFileSystem();
};

STATIC_ASSERT(sizeof(PackFileSystem) == 0xFC);
