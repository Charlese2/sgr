#include <game/CrankyFileManager.h>

#define MAX_PARSE_FILENAME_LEN 64

class parse {
  private:
    char *m_pFileName;
    int unk4;
    char m_fileName[MAX_PARSE_FILENAME_LEN];
    u32 m_line;
    CrankyFileBuffer m_fileBuff;
    u32 *m_pBuffer;
    u8 *m_pData;
    int unkE4;

  public:
    void ParseFile(char *fname, int path_index, u8 *data, int unk);
};
