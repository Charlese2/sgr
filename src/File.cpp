#include "game/File.h"
#include "game/NGCSystem.h"
#include "game/FileSystem.h"
#include "macros.h"
#include "dolphin/os.h"
#include <string.h>

File::File(){
    initialize();
};

File::~File(){
    close();
};

void File::initialize() {
    m_pathId = 0;
    m_fileRec = -1;
}

BOOL File::get(const char *filename, int pathId) {
    if (m_fileRec != -1) {
        close();
    }
    strncpy(m_fileName, filename, sizeof(m_fileName));
    m_fileName[47] = '\0';
    m_pathId = pathId;
    m_fileRec = gFileManager.OpenNewFile(GetFilePath(m_pathId), filename);
    if (m_fileRec == -1) {
        return TRUE;
    }
    DEBUGASSERTLINE(92, NULL == gFileManager.GetFileFromCache((char*)GetFilePath(m_pathId), (char*)filename));
    return FALSE;
}

s32 File::size(const char *file_name, int pathIndex) {
    int file_record;
    u32 file_size;
    if (file_name) {
        file_record = gFileManager.OpenNewFile(GetFilePath(pathIndex), file_name);
        file_size   = gFileManager.GetFileSize(file_record);
        gFileManager.CloseFile(file_record);
        return file_size;
    } else {
        ASSERTLINE(116, m_fileRec != kInvalidFileRecord);
        return OSRoundUp32B(gFileManager.GetFileSize(m_fileRec));
    }
}

bool File::close() {
    if (m_fileRec != -1) {
        gFileManager.CloseFile(m_fileRec);
        initialize();
    } 
    return 1;
}

BOOL File::read(u32* buf, s32 bufSize) {
    ASSERTLINE(172, m_fileRec != kInvalidFileRecord);
    ASSERTLINE(173, buf);
    ASSERTLINE(174, bufSize >= size());
    NGCSystem::DriveStatus(0, 0);
    FileTypeCallback file_type_callback = NULL;
    size_t length = strlen(m_fileName);
    char *file_type = m_fileName + length - 4;
    if (stricmp( file_type, ".pkf") == 0) {
        file_type_callback = PkfFileTypeHandler;
    }
    gFileManager.ReadFile(m_fileRec, buf, file_type_callback);
    NGCSystem::DriveStatus(0, 0);
    return TRUE;
}
