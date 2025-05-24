#include <game/CrankyFileManager.h>

class parse {
private:
	char* m_fileName;
	int unk4;
	char unk8[56];
	u32 m_line;
	CrankyFileBuffer m_fileBuff;
	u8* m_pBuffer;
	u8* m_pData;
	int unkE4;

public:
	void ParseFile(char* fname, int path_index, u8* data, int unk);
};
