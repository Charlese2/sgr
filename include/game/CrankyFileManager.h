#ifndef _GAME_CRANKYFILEMANAGER_H_
#define _GAME_CRANKYFILEMANAGER_H_

#include "global.h"
extern "C" {
#include "dolphin/dvd.h"
}

typedef void(FileFoundCallback)();
typedef void(FileMissingCallback)(char *file);
typedef bool (*FileTypeCallback)(int *fileTypeTable);

class CrankyFile {
  public:
    virtual ~CrankyFile();
    CrankyFile();
    void OpenFile(const char *file_path, const char *file_name);
    void CloseFile(void);
    u32 GetFileSize(void);
    void SetReadPosition(int startPos, u32 mode);
    int ReadFile(u32 *data, int length);
    BOOL IsOpened() const { return m_Opened; };
    u32 GetPosition(void) const { return m_position; };
    int GetDiskFileSize(u8 *data);

  private:
    DVDFileInfo m_fileInfo;
    u32 m_position;
    BOOL m_Opened;
    char m_filePath[8];
    char m_fileName[48];
};

class CrankyFileCache {};

class CrankyFileBuffer : CrankyFile {
  public:
    void SetFileBuffer(u32 *buf, int requested_size);
    BOOL IsActive() const { return IsOpened(); };

  private:
    int unk80;
    u32 m_TotalSize;
    u32 m_BufferSize;
    u32 *m_pBuffer;
    int unk90;
};

class CrankyFileManager {
  public:
    virtual ~CrankyFileManager();
    CrankyFileManager();
    int OpenNewFile(const char *file_name, const char *file_path);
    u32 GetFileSize(int file_record);
    u32 GetFileFromCache(const char *file_path, char* file_name);
    void ReadFile(int file_record, u32 *buffer, FileTypeCallback callback);
    int CloseFile(int file_record);
    void SetMissingFileCallback(FileMissingCallback *callback);

  private:
    CrankyFileCache *m_fileCache;
    u32 m_availableFileCaches;
    FileTypeCallback SomethingFileTypes;
    int *m_pFileTypeTable;
    u32 m_RegisteredFileTypes;
    u32 m_MaxFiletypeLimit;
    CrankyFile *m_pFile;
    u32 m_MaxNumberOfOpenFiles;
};

void set_missing_file_callback(FileMissingCallback *callback);

void ReadFileCallback(void);

STATIC_ASSERT(sizeof(CrankyFileManager) == 0x24);

#endif // _GAME_CRANKYFILEMANAGER_H_
