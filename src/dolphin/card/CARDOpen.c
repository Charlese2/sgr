#include "dolphin/card/CARDOpen.h"
#include "dolphin/card.h"
#include "dolphin/card/CARDPriv.h"
#include "dolphin/dvd.h"

/* 80357C74-80357CDC 3525B4 0068+00 1/1 1/1 0/0 .text            __CARDCompareFileName */
BOOL __CARDCompareFileName(CARDDir* ent, const char* fileName) {
    char* entName;
    char c1;
    char c2;
    int n;

    entName = (char*)ent->fileName;
    n = CARD_FILENAME_MAX;
    while (0 <= --n) {
        if ((c1 = *entName++) != (c2 = *fileName++)) {
            return FALSE;
        } else if (c2 == '\0') {
            return TRUE;
        }
    }

    if (*fileName == '\0') {
        return TRUE;
    }

    return FALSE;
}

/* 80357CDC-80357D70 35261C 0094+00 2/2 0/0 0/0 .text            __CARDAccess */
s32 __CARDAccess(CARDControl* card, CARDDir* ent) {
    const DVDDiskID* diskId = card->diskID;
    if (ent->gameName[0] == 0xFF) {
        return CARD_RESULT_NOFILE;
    }

    if (diskId == &__CARDDiskNone || (memcmp(ent->gameName, diskId->game_name, 4) == 0 &&
                                      memcmp(ent->company, diskId->company, 2) == 0))
    {
        return CARD_RESULT_READY;
    }

    return CARD_RESULT_NOPERM;
}

/* 80357D70-80357EA4 3526B0 0134+00 0/0 2/2 0/0 .text            __CARDIsWritable */
s32 __CARDIsWritable(CARDControl* card, CARDDir* ent) {
    s32 retVal;
    const DVDDiskID* diskId = card->diskID;
    u8 val;
    if ((u8)ent->gameName[0] == 0xFFu) {
        retVal = CARD_RESULT_NOFILE;
    } else if (diskId == &__CARDDiskNone || (memcmp(ent->gameName, diskId->game_name, 4) == 0 &&
                                             memcmp(ent->company, diskId->company, 2) == 0))
    {
        retVal = CARD_RESULT_READY;
    } else {
        retVal = CARD_RESULT_NOPERM;
    }

    if (retVal == CARD_RESULT_NOPERM) {
        val = ent->permission & data_80450A72;
        if (val & 0x20u && (memcmp(ent->gameName, __CARDDiskNone.game_name, 4) == 0 &&
                            memcmp(ent->company, __CARDDiskNone.company, 2) == 0))
        {
            return CARD_RESULT_READY;
        } else if (val & 0x40 && (memcmp(ent->gameName, __CARDDiskNone.game_name, 4) == 0 &&
                                  memcmp(ent->company, diskId->company, 2) == 0))
        {
            return CARD_RESULT_READY;
        }
    }

    return retVal;
}

/* 80357EA4-80357F98 3527E4 00F4+00 0/0 2/2 0/0 .text            __CARDIsReadable */
s32 __CARDIsReadable(CARDControl* card, CARDDir* ent) {
    u8 val;
    s32 retVal;
    const DVDDiskID* diskId = card->diskID;

    retVal = __CARDAccess(card, ent);

    if (retVal == CARD_RESULT_NOPERM) {
        val = ent->permission & data_80450A72;
        if (val & 0x20u && (memcmp(ent->gameName, __CARDDiskNone.game_name, 4) == 0 &&
                            memcmp(ent->company, __CARDDiskNone.company, 2) == 0))
        {
            retVal = CARD_RESULT_READY;
        } else if (val & 0x40 && (memcmp(ent->gameName, __CARDDiskNone.game_name, 4) == 0 &&
                                  memcmp(ent->company, diskId->company, 2) == 0))
        {
            retVal = CARD_RESULT_READY;
        }
    }

    if (retVal == CARD_RESULT_NOPERM && (ent->permission & 0x4)) {
        return CARD_RESULT_READY;
    }

    return retVal;
}

static inline s32 __CARDGetFileNo(CARDControl* card, const char* fileName, s32* pfileNo) {
    CARDDir* dir;
    CARDDir* ent;
    s32 fileNo;
    s32 result;

    if (!card->attached) {
        return CARD_RESULT_NOCARD;
    }

    dir = __CARDGetDirBlock(card);
    for (fileNo = 0; fileNo < CARD_MAX_FILE; fileNo++) {
        ent = &dir[fileNo];
        result = __CARDAccess(card, ent);
        if (result < 0) {
            continue;
        }
        if (__CARDCompareFileName(ent, fileName)) {
            *pfileNo = fileNo;
            return CARD_RESULT_READY;
        }
    }

    return CARD_RESULT_NOFILE;
}

/* 80357F98-803580B4 3528D8 011C+00 0/0 3/3 0/0 .text            CARDOpen */
s32 CARDOpen(s32 chan, const char* fileName, CARDFileInfo* fileInfo) {
    CARDControl* card;
    CARDDir* dir;
    CARDDir* ent;
    s32 result;
    s32 fileNo;

    fileInfo->chan = -1;
    result = __CARDGetControlBlock(chan, &card);
    if (result < 0) {
        return result;
    }
    result = __CARDGetFileNo(card, fileName, &fileNo);
    if (0 <= result) {
        dir = __CARDGetDirBlock(card);
        ent = &dir[fileNo];
        if (!CARDIsValidBlockNo(card, ent->startBlock)) {
            result = CARD_RESULT_BROKEN;
        } else {
            fileInfo->chan = chan;
            fileInfo->fileNo = fileNo;
            fileInfo->offset = 0;
            fileInfo->iBlock = ent->startBlock;
        }
    }
    return __CARDPutControlBlock(card, result);
}

/* 803580B4-80358108 3529F4 0054+00 0/0 3/3 0/0 .text            CARDClose */
s32 CARDClose(CARDFileInfo* fileInfo) {
    CARDControl* card;
    s32 result;

    result = __CARDGetControlBlock(fileInfo->chan, &card);
    if (result < 0) {
        return result;
    }

    fileInfo->chan = -1;
    return __CARDPutControlBlock(card, CARD_RESULT_READY);
}