#ifndef _GAME_PACKFILESYSTEM_H_
#define _GAME_PACKFILESYSTEM_H_

#include "global.h"
#include "game/CrankyFileManager.h"

class PackFileSystem {
  public:
    virtual ~PackFileSystem();
    PackFileSystem();

    int add_pack_file(const char *pak_file, int unk1, int unk2, int unk3);
    void set_current_file_data(u8 *data);
    int ReadFileFromPack(char *pak_file, u32 file_path_index, u8 *destination, u32 buffSize);

  private:
    int m_curFile;
    int unk8;
    class FileList {
      u8 * m_pFileData;
      int unk4;
      int unk8;
    } m_fileList[20];
};

extern PackFileSystem gPackFileSystem;

STATIC_ASSERT(sizeof(PackFileSystem) == 0xFC);

#endif // _GAME_PACKFILESYSTEM_H_
