#include "game/Cranky.h"
#include "game/CrankyFileManager.h"
#include "dolphin/os.h"
#include "macros.h"

extern FileFoundCallback *file_found_callback;
extern FileMissingCallback *file_missing_callback;

CrankyFile::CrankyFile() { m_Opened = 0; }

CrankyFile::~CrankyFile() {}

void set_missing_file_callback(FileMissingCallback *callback) { file_missing_callback = callback; }

void set_file_found_callback(FileFoundCallback *callback) { file_found_callback = callback; }

void CrankyFile::OpenFile(char *file_path, char *file_name) {
    char file[56];
    char buffer[56];
    ASSERTMSGLINE(87, strlen(file_path) < 8, "path name exceeds maximum allowable length");
    ASSERTMSGLINE(88, strlen(file_name) < 48, "file name exceeds maximum allowable length");
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

void CrankyFile::ReadFile(u8 *buffer, int size) {
    
}

void CrankyFile::CloseFile(void) {
    DVDClose(&m_fileInfo);
    m_Opened = 0;
}

u32 CrankyFile::GetFileSize(void) { return m_fileInfo.length; };

void CrankyFile::unk0() {}
