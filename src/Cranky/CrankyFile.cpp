#include "game/Cranky.h"
#include "game/CrankyFileManager.h"
#include "dolphin/os.h"
#include "macros.h"

#pragma optimize_for_size off // (-opt speed)

void CrankyFile::OpenFile(char* file_path, char* file_name) {
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
        if (file_not_found_callback) {
            file_not_found_callback(buffer);
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
