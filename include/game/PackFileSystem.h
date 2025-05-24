#include "global.h"
#include "game/CrankyFileManager.h"

class PackFileSystem {
    virtual void unk0();
    int unk4;
    int unk8[61];

    
    public:
    PackFileSystem();
    ~PackFileSystem();
};

STATIC_ASSERT(sizeof(PackFileSystem) == 0xFC);
