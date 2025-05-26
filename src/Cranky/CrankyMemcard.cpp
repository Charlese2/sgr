#include "game/CrankyMemcard.h"
#include "dolphin/card.h"
#include "dolphin/card/CARDDelete.h"
#include <string.h>

CrankyMemcard::CrankyMemcard() {
    m_cardWorkArea = NULL;
    m_activeSlot   = 0;
    m_memSize      = -1;
    m_sectorSize   = -1;
    m_isMounted    = false;
    m_startPos     = 0;
    memset(m_gameName, 0, 4);
    memset(m_company, 0, 2);
}

CrankyMemcard::~CrankyMemcard() { ASSERTLINE(47, !m_isMounted); }

void CrankyMemcard::NewMemcard(int *cardWorkArea) {
    CARDInit();
    m_cardWorkArea = cardWorkArea;
}

void CrankyMemcard::SetActiveSlot(int cardId) {
    ASSERTLINE(76, cardId >= 0 && cardId < kMemcardMaxSlots);
    if (IsMounted()) {
        Unmount();
    }
    m_activeSlot = cardId;
}

int CrankyMemcard::ProbeInfo() {
    s32 result;
    ASSERTLINE(102, m_activeSlot >= 0 && m_activeSlot < kMemcardMaxSlots);
    ASSERTLINE(103, !IsMounted());
    while ((result = CARDProbeEx(m_activeSlot, &m_memSize, &m_sectorSize)) == -1) {
    };
    m_memSize <<= 17;
    if (result != CARD_RESULT_READY) {
        return ConvertResult(result);
    }
    return 0;
}

int CrankyMemcard::GetState(CARDCallback detachCallback) {
    s32 result;
    ASSERTLINE(159, m_activeSlot >= 0 && m_activeSlot < kMemcardMaxSlots);
    ASSERTLINE(160, m_cardWorkArea);
    ASSERTLINE(161, !IsMounted());

    if ((result = ProbeInfo()) != 0) {
        return result;
    }
    result = CARDMount(m_activeSlot, m_cardWorkArea, detachCallback);

    switch (result) {
    case CARD_RESULT_BUSY:
    case CARD_RESULT_WRONGDEVICE:
    case CARD_RESULT_NOCARD:
    case CARD_RESULT_NOFILE:
    case CARD_RESULT_IOERROR:
    case CARD_RESULT_EXIST:
    case CARD_RESULT_NOENT:
    case CARD_RESULT_INSSPACE:
    case CARD_RESULT_NOPERM:
    case CARD_RESULT_LIMIT:
    case CARD_RESULT_NAMETOOLONG:
    case CARD_RESULT_CANCELED:
        return ConvertResult(result);
    case CARD_RESULT_READY:
    case CARD_RESULT_BROKEN:
    case CARD_RESULT_ENCODING:
        m_isMounted = true;
        if ((result = CARDCheck(m_activeSlot)) == CARD_RESULT_NOCARD) {
            m_isMounted = false;
        }
    }

    return ConvertResult(result);
}

int CrankyMemcard::Unmount() {
    ASSERTLINE(210, m_activeSlot >= 0 && m_activeSlot < kMemcardMaxSlots);
    if (!IsMounted()) {
        return 0;
    } else {
        m_isMounted = false;
        return ConvertResult(CARDUnmount(m_activeSlot));
    }
}

int CrankyMemcard::Format(CARDCallback formatCallback) {
    ASSERTLINE(250, m_activeSlot >= 0 && m_activeSlot < kMemcardMaxSlots);
    return ConvertResult(CARDFormatAsync(m_activeSlot, formatCallback));
}

int CrankyMemcard::Free(s32 *filesNotUsed, s32 *bytesNotUsed) {
    ASSERTLINE(266, m_activeSlot >= 0 && m_activeSlot < kMemcardMaxSlots);
    if (CARDFreeBlocks(m_activeSlot, bytesNotUsed, filesNotUsed)) {
        if (filesNotUsed) {
            *filesNotUsed = 0;
        }
        if (bytesNotUsed) {
            *bytesNotUsed = 0;
        }
    }
}

int CrankyMemcard::GetChunkSize(int size) {
    ASSERTLINE(314, m_activeSlot >= 0 && m_activeSlot < kMemcardMaxSlots);
    if (!IsMounted()) {
        GetState(NULL);
        Unmount();
    }
    ASSERTLINE(322, m_sectorSize > 0);
    return (size + m_sectorSize - 1) / m_sectorSize;
}

int CrankyMemcard::SetStatus(CARDFileInfo *fileInfo, CARDStat *status) {
    return ConvertResult(CARDSetStatus(fileInfo->chan, fileInfo->fileNo, status));
}

int CrankyMemcard::GetStatus(CARDFileInfo *fileInfo, CARDStat *status) {
    return ConvertResult(CARDGetStatus(fileInfo->chan, fileInfo->fileNo, status));
}

int CrankyMemcard::Open(CARDFileInfo *fileInfo, char *filename) {
    ASSERTLINE(365, m_activeSlot >= 0 && m_activeSlot < kMemcardMaxSlots);
    ASSERTLINE(366, fileInfo);
    return ConvertResult(CARDOpen(m_activeSlot, filename, fileInfo));
}

int CrankyMemcard::Close(CARDFileInfo *fileInfo) {
    ASSERTLINE(382, fileInfo);
    ASSERTLINE(383, fileInfo->chan == m_activeSlot);
    return ConvertResult(CARDClose(fileInfo));
}

int CrankyMemcard::Unknown(CARDFileInfo *fileInfo, CARDStat *status) {
    int result;
    CARDStat _status;
    if (!IsMounted() || fileInfo->chan != m_activeSlot) {
        SetActiveSlot(fileInfo->chan);
        result = CrankyMemcard::GetState(NULL);
        if (result != CARD_RESULT_READY) {
            return ConvertResult(result);
        }
    }
    result              = CARDGetStatus(m_activeSlot, fileInfo->fileNo, &_status);
    status->fileName[0] = _status.length;
    return ConvertResult(result);
}

int CrankyMemcard::Create(CARDFileInfo *fileInfo, char *fileName, u32 size, CARDStat *status) {
    int result;
    ASSERTLINE(430, m_activeSlot >= 0 && m_activeSlot < kMemcardMaxSlots);
    ASSERTLINE(431, fileInfo);
    result = CARDCreate(m_activeSlot, fileName, size, fileInfo);
    if (result != CARD_RESULT_READY || status == NULL) {
        return ConvertResult(result);
    }
    return ConvertResult(CARDSetStatus(fileInfo->chan, fileInfo->fileNo, status));
}

int CrankyMemcard::Read(CARDFileInfo *fileInfo, void *buffer, u32 length, u32 offset) {
    int result;
    if (!IsMounted() || fileInfo->chan != m_activeSlot) {
        SetActiveSlot(fileInfo->chan);
        result = GetState(NULL);
        if (result != CARD_RESULT_READY) {
            return ConvertResult(result);
        }
    }
    return ConvertResult(CARDRead(fileInfo, buffer, length, offset));
}

int CrankyMemcard::ReadAsync(CARDFileInfo *fileInfo, void *buffer, u32 length, u32 offset, CARDCallback callback) {
    int result;
    if (!IsMounted() || fileInfo->chan != m_activeSlot) {
        SetActiveSlot(fileInfo->chan);
        result = GetState(NULL);
        if (result != CARD_RESULT_READY) {
            return ConvertResult(result);
        }
    }
    return ConvertResult(CARDReadAsync(fileInfo, buffer, length, offset, callback));
}

int CrankyMemcard::WriteAsync(CARDFileInfo *fileInfo, void *buffer, u32 length, u32 offset, CARDCallback callback) {
    int result;
    if (!IsMounted() || fileInfo->chan != m_activeSlot) {
        SetActiveSlot(fileInfo->chan);
        result = GetState(NULL);
        if (result != CARD_RESULT_READY) {
            return ConvertResult(result);
        }
    }
    return ConvertResult(CARDWriteAsync(fileInfo, buffer, length, offset, callback));
}

int CrankyMemcard::Delete(char *fileName) { return ConvertResult(CARDDelete(m_activeSlot, fileName)); }

int CrankyMemcard::SetSaveInfo(char *gameName, char *company) {
    memcpy(m_gameName, gameName, 4);
    memcpy(m_company, company, 2);
}

int CrankyMemcard::Unknown2(CARDStat *status, int startPos) {
    ASSERTLINE(582, m_activeSlot >= 0 && m_activeSlot < kMemcardMaxSlots);
    ASSERTLINE(583, IsMounted());
    ASSERTLINE(584, startPos >= 0);
    m_startPos = startPos;
    for (; m_startPos < 127; m_startPos++) {
        if ((CARDGetStatus(m_activeSlot, m_startPos, status)) >= CARD_RESULT_READY) {
            if (memcmp(status->gameName, m_gameName, 4) == 0 && memcmp(status->company, m_company, 2) == 0) {
                return m_startPos++;
            }
        }
    }
    return -1;
}

int CrankyMemcard::ConvertResult(int result) {
    switch (result) {
    case CARD_RESULT_READY:
        return 0;
    case CARD_RESULT_BUSY:
        return 8;
    case CARD_RESULT_WRONGDEVICE:
        return 5;
    case CARD_RESULT_NOCARD:
        return 1;
    case CARD_RESULT_IOERROR:
        return 4;
    case CARD_RESULT_BROKEN:
        return 2;
    case CARD_RESULT_ENCODING:
        return 3;
    case CARD_RESULT_NOFILE:
        return 9;
    default:
        return 10;
    }
}

int CrankyMemcard::GetResultCode(void) {
    ASSERTLINE(677, m_activeSlot >= 0 && m_activeSlot < kMemcardMaxSlots);
    return ConvertResult(CARDGetResultCode(m_activeSlot));
}
