#include "game/File.h"
#include "game/FileSystem.h"
#include "macros.h"
#include "dolphin/os.h"

File::File(){};

File::~File(){};

bool File::GetFile(char *file_name, int file_path_index) {
    return 0;
}

u32 File::GetFileSize(char *file, int pathIndex) {
    int file_record;
    u32 file_size;
    if (file) {
        file_record = gFileManager.OpenNewFile(GetFilePath(pathIndex), file);
        file_size   = gFileManager.GetFileSize(file_record);
        gFileManager.CloseFile(file_record);
        return file_size;
    } else {
        ASSERTLINE(116, m_fileRec != kInvalidFileRecord);
        return OSRoundUp32B(gFileManager.GetFileSize(m_fileRec));
    }
}
