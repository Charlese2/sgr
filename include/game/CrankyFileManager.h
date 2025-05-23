#include "dolphin/dvd.h"

class CrankyFile {
private:
	char** unk0;
	DVDFileInfo m_fileInfo;
	u32 m_position;
	BOOL m_Opened;
	char m_filePath[8];
	char m_fileName[48];

public:
	BOOL IsOpened() const { return m_Opened; };
};

class CrankyFileBuffer {
private:
	CrankyFile m_file;
	int unk80;
	u32 m_TotalSize;
	u32 m_BufferSize;
	char* m_pBuffer;
	int unk90;

public:
	BOOL IsActive() const { return m_file.IsOpened() == true; };
};

class CrankyFileManager {

};

extern CrankyFileManager gFileManager;
