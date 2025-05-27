#include "game/CrankyFileManager.h"
#include "dolphin/os.h"
#include "game/macros.h"


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

int CrankyFileManager::OpenNewFile(char* file_name, char* file_path) {
    for (int file_index = 0; file_index < m_MaxNumberOfOpenFiles; file_index++) {
        if (!m_pFile[file_index].IsOpened()) {
            m_pFile[file_index].OpenFile(file_name, file_path);
            return file_index;
        }
    }

    ASSERTMSGLINE(135, false, "maximum open files exceeded");
    return -1;
}

void CrankyFileManager::SetMissingFileCallback(FileMissingCallback* callback) {
    set_missing_file_callback(callback);
}
