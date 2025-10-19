#include "game/Cranky.h"
#include "game/CrankyFileManager.h"
#include "game/File.h"
#include "dolphin/os.h"
#include "macros.h"

BOOL readingFile;
FileFoundCallback *file_found_callback;
FileMissingCallback *file_missing_callback;

CrankyFile::CrankyFile() { m_Opened = 0; }

CrankyFile::~CrankyFile() {}

void set_missing_file_callback(FileMissingCallback *callback) { file_missing_callback = callback; }

void set_file_found_callback(FileFoundCallback *callback) { file_found_callback = callback; }

void CrankyFile::OpenFile(const char *file_path, char *file_name) {
    char file[56];
    char buffer[56];
    ASSERTMSGLINE(87, strlen(file_path) < 8, "path name exceeds maximum allowable length");
    ASSERTMSGLINE(88, strlen(file_name) < MAX_FILE_NAME_LENGTH, "file name exceeds maximum allowable length");
    ASSERTMSGLINE(89, !m_Opened, "previous file not closed");
    strcpy(file, file_path);
    strcat(file, file_name);
    strcpy(m_filePath, file_path);
    strcpy(m_fileName, file_name);
    if (!DVDOpen(file, &m_fileInfo)) {
        sprintf(buffer, "%s/%s not found", file_path, file_name);
        if (file_missing_callback) {
            file_missing_callback(buffer);
        } else {
            ASSERTMSGLINE(106, false, buffer);
        }
    } else {
        m_Opened = TRUE;
    }

    if (file_found_callback) {
        file_found_callback();
    }

    m_position = 0;
}

void CrankyFile::CloseFile(void) {
    DVDClose(&m_fileInfo);
    m_Opened = FALSE;
}

u32 CrankyFile::GetFileSize(void) { return m_fileInfo.length; };

void CrankyFile::SetReadPosition(int startPos, u32 mode) {
    int position;

    if (mode == 0)
        position = 0;
    else if (mode == 1)
        position = GetPosition();
    else if (mode == 2)
        position = GetFileSize();
    else {
        ASSERTLINE(175, 0);
    }
    m_position = position + startPos;
    ASSERTLINE(181, m_position <= this->GetFileSize());
}

int CrankyFile::ReadFile(u32 *data, int length) {
    u32 size;
    u32 position;
    u32 result;
    u32 file_size;
    u32 new_size;

    ASSERTLINE(202, CrankyTestAlign32((u32)data));
    ASSERTLINE(203, CrankyTestAlign32((u32)length));
    ASSERTLINE(204, m_position < this->GetFileSize());
    position  = m_position + length;
    file_size = GetFileSize();
    if (position > file_size) {
        new_size = file_size - m_position;
    } else {
        new_size = length;
    }
    size = new_size;
    size = CrankyRoundUp32(size);
    ASSERTLINE(213, CrankyTestAlign4(m_position));
    result = 0;
    if (file_found_callback) {
        readingFile = TRUE;
        if (DVDReadAsyncPrio(&m_fileInfo, data, size, m_position, (DVDCallback)ReadFileCallback, 2)) {
            while (readingFile) {
                file_found_callback();
            }
            result = size;
        }
    } else {
        result = DVDReadPrio(&m_fileInfo, data, size, m_position, 2);
    }
    m_position += result;
    return result;
}

int CrankyFile::GetDiskFileSize(u8 *data) {
    if (file_found_callback) {
        readingFile = TRUE;
        if (DVDReadAsyncPrio(&m_fileInfo, data, OSRoundUp32B(GetFileSize()), 0, (DVDCallback)ReadFileCallback, 2)) {
            while (readingFile) {
                file_found_callback();
            }
            return OSRoundUp32B(GetFileSize());
        } else {
            return 0;
        }
    } else {
        return DVDReadPrio(&m_fileInfo, data, OSRoundUp32B(GetFileSize()), 0, 2);
    }
}

void ReadFileCallback() { readingFile = FALSE; }
