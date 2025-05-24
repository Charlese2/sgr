#include "game/FileSystem.h"

extern "C" {
	#include "dolphin/dvd.h"
}
extern FileFoundCallback* file_found_callback;
extern FileNotFoundCallback* file_not_found_callback;

class CrankyFile {
private:
	char** unk0;
	DVDFileInfo m_fileInfo;
	u32 m_position;
	BOOL m_Opened;
	char m_filePath[8];
	char m_fileName[48];

public:
	void OpenFile(char* file_path, char* file_name);
	BOOL IsOpened() const { return m_Opened; };
};

class CrankyFileBuffer {
private:
	CrankyFile m_file;
	int unk80;
	u32 m_TotalSize;
	u32 m_BufferSize;
	u8* m_pBuffer;
	int unk90;

	

public:
	void SetFileBuffer(u8* buf, int requested_size);
	BOOL IsActive() const { return m_file.IsOpened(); };
};

class CrankyFileManager {
	void set_missing_file_callback(FileNotFoundCallback* callback);
	public:
	
	void SetMissingFileCallback(FileNotFoundCallback* callback);
};

extern CrankyFileManager gFileManager;
