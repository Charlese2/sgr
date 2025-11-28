#include <dolphin/types.h>

#define MAX_FILE_NAME_LENGTH 48

#define kInvalidFileRecord -1

class File {
  public:
    File();
    ~File();

    void initialize(void);
    BOOL get(const char *filename, int filePath);
    s32 size(const char *file = NULL, int pathIndex = 0);
    bool close(void);
    BOOL read(u32* buf, s32 bufSize);

    char m_fileName[48];
    int m_pathId;
    u32 m_fileRec;
};
