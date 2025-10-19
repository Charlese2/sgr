#include "global.h"
#include "game/CrankyFileManager.h"

class PackFileSystem {
  public:
    virtual ~PackFileSystem();
    PackFileSystem();

    int ReadFileFromPack(char *pak_file, u32 file_path_index, u8 *destination, u32 buffSize);

  private:
    int unk4;
    int unk8[61];
};

extern PackFileSystem gPackFileSystem;

STATIC_ASSERT(sizeof(PackFileSystem) == 0xFC);
