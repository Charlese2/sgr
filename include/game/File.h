#include <dolphin/types.h>

#define MAX_FILE_NAME_LENGTH 48

#define kInvalidFileRecord -1

class File {
  public:
    File();
    ~File();

    bool GetFile(char *filename, int filePath);
    u32 GetFileSize(char *file, int pathIndex);
    void CloseFile(void);

    char m_fileName[48];
    int m_filePath;
    u32 m_fileRec;
};
