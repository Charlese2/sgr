#include "global.h"
#include "game/CrankyFileManager.h"

class PackFileSystem {
  public:
    virtual ~PackFileSystem();
    PackFileSystem();

  private:
    int unk4;
    int unk8[61];
};

STATIC_ASSERT(sizeof(PackFileSystem) == 0xFC);
