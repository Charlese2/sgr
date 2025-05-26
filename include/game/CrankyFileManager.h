#include "global.h"
extern "C" {
#include "dolphin/dvd.h"
}

typedef void(FileFoundCallback)();
typedef void(FileMissingCallback)(char *file);
typedef bool (*FileTypeCallback)(int *fileTypeTable);

extern FileFoundCallback *file_found_callback;
extern FileMissingCallback *file_missing_callback;

class CrankyFile {
  private:
  virtual void unk0();
  DVDFileInfo m_fileInfo;
  u32 m_position;
  BOOL m_Opened;
  char m_filePath[8];
  char m_fileName[48];
  
  public:
  void OpenFile(char *file_path, char *file_name);
  void CloseFile(void);
  u32 GetFileSize(void);
  void ReadFile(u8 *buffer, int size);
  BOOL IsOpened() const { return m_Opened; };
  u32 GetPosition(void) const { return m_position; };
  CrankyFile();
  ~CrankyFile();
};

class CrankyFileCache {};

class CrankyFileBuffer : CrankyFile {
  private:
  int unk80;
  u32 m_TotalSize;
  u32 m_BufferSize;
  u8 *m_pBuffer;
  int unk90;
  
  public:
  void SetFileBuffer(u8 *buf, int requested_size);
  BOOL IsActive() const { return IsOpened(); };
};

class CrankyFileManager {
  public:
  virtual void unk0();
  CrankyFileCache *m_fileCache;
  u32 m_availableFileCaches;
  FileTypeCallback SomethingFileTypes;
  int *m_pFileTypeTable;
  u32 m_RegisteredFileTypes;
  u32 m_MaxFiletypeLimit;
  CrankyFile *m_pFile;
  u32 m_MaxNumberOfOpenFiles;
  
  public:
  CrankyFileManager();
  ~CrankyFileManager();
  int OpenNewFile(char *file_name, char *file_path);
  void SetMissingFileCallback(FileMissingCallback *callback);
};

void set_missing_file_callback(FileMissingCallback *callback);

STATIC_ASSERT(sizeof(CrankyFileManager) == 0x24);
