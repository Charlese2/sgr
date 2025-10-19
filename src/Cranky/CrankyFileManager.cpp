#include "game/CrankyFileManager.h"
#include "dolphin/os.h"
#include "macros.h"


CrankyFileManager::CrankyFileManager() {
    m_fileCache = NULL;
    m_availableFileCaches = 0;
    SomethingFileTypes = NULL;
    m_pFileTypeTable = NULL;
    m_MaxFiletypeLimit = 0;
    m_RegisteredFileTypes = 0;
    m_pFile = NULL;
    m_MaxNumberOfOpenFiles = 0;
}

CrankyFileManager::~CrankyFileManager() {
    if (SomethingFileTypes) {
        SomethingFileTypes(m_pFileTypeTable);
        m_fileCache = 0;
        m_availableFileCaches = 0;
        m_pFileTypeTable = NULL;
        m_MaxFiletypeLimit = 0;
        m_RegisteredFileTypes = 0;
        m_pFile = NULL;
        m_MaxNumberOfOpenFiles = 0;
        SomethingFileTypes = NULL;
    }
}

int CrankyFileManager::OpenNewFile(const char* file_name, char* file_path) {
    for (int file_index = 0; file_index < m_MaxNumberOfOpenFiles; file_index++) {
        if (!m_pFile[file_index].IsOpened()) {
            m_pFile[file_index].OpenFile(file_name, file_path);
            return file_index;
        }
    }

    ASSERTMSGLINE(135, false, "maximum open files exceeded");
    return -1;
}

u32 CrankyFileManager::GetFileSize(int file_record) {
    u32 file_size = 1;
    if (!m_pFile[file_record].IsOpened()) {
        ASSERTMSGLINE(167, false, "accessing a file that has not been opened");
    }
    file_size = m_pFile[file_record].GetFileSize();
    return file_size;
}

void CrankyFileManager::SetMissingFileCallback(FileMissingCallback* callback) {
    set_missing_file_callback(callback);
}
