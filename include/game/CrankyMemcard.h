#include "dolphin/card.h"
#include "global.h"

const int kMemcardMaxSlots = 2;

class CrankyMemcard {
    virtual ~CrankyMemcard();
    int *m_cardWorkArea;
    int m_startPos;
    char m_gameName[4];
    char m_company[2];
    int m_activeSlot;
    s32 m_memSize;
    s32 m_sectorSize;
    bool m_isMounted;

  public:
    CrankyMemcard();
    void NewMemcard(int *cardWorkArea);
    void SetActiveSlot(int cardId);
    int ProbeInfo(void);
    int GetState(CARDCallback detachCallback);
    int Unmount(void);
    int Format(CARDCallback formatCallback);
    void Free(s32 *filesNotUsed, s32 *bytesNotUsed);
    int GetBlockSize(int size);
    int SetStatus(CARDFileInfo *fileInfo, CARDStat *status);
    int GetStatus(CARDFileInfo *fileInfo, CARDStat *status);
    int Open(CARDFileInfo *fileInfo, char *filename);
    int Close(CARDFileInfo *fileInfo);
    int GetSaveLength(CARDFileInfo *fileInfo, u32 *length);
    int Create(CARDFileInfo *fileInfo, char *fileName, u32 size, CARDStat *status);
    int Read(CARDFileInfo *fileInfo, void *buffer, u32 length, u32 offset);
    int ReadAsync(CARDFileInfo *fileInfo, void *buffer, u32 length, u32 offset, CARDCallback callback);
    int WriteAsync(CARDFileInfo *fileInfo, void *buffer, u32 length, u32 offset, CARDCallback callback);
    int Delete(char *fileName);
    void SetSaveInfo(char *gameName, char *company);
    int FindNextSaveFile(CARDStat *status, int startPos);
    int ConvertResult(int result);
    int GetResultCode(void);
    bool IsMounted(void) { return m_isMounted; };
};

STATIC_ASSERT(sizeof(CrankyMemcard) == 0x24);
