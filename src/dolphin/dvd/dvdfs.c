#include "dolphin/dvd/dvdfs.h"
#include "dolphin/dvd.h"
#include "dolphin/os.h"

void __DVDFSInit();
int DVDConvertPathToEntrynum(const char* path);
int DVDFastOpen(long entryNum, DVDFileInfo* info);
BOOL DVDOpen(const char* filename, DVDFileInfo* fileinfo);
BOOL DVDClose(DVDFileInfo* fileinfo);
static u32 entryToPath(u32 entry, char* path, u32 maxlen);
static BOOL DVDConvertEntrynumToPath(s32 entrynum, char* path, u32 maxlen);
static BOOL DVDGetCurrentDir(char* path, u32 maxlen);
BOOL DVDChangeDir(const char* dirname);
BOOL DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset,
                      DVDCallback callback, s32 prio);
static void cbForReadAsync();
int DVDReadPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, s32 prio);
static void cbForReadSync();
BOOL DVDOpenDir(const char*, DVDDirectory* dir);
BOOL DVDReadDir(DVDDirectory* dir, DVDDirectoryEntry* entry);
BOOL DVDCloseDir();

typedef struct FSTEntry FSTEntry;

struct FSTEntry {
    unsigned int isDirAndStringOff;
    unsigned int parentOrPosition;
    unsigned int nextEntryOrLength;
};

/* 80451758-8045175C 000C58 0004+00 1/1 0/0 0/0 .sbss            BootInfo */
static OSBootInfo* BootInfo;

/* 8045175C-80451760 000C5C 0004+00 10/10 0/0 0/0 .sbss            FstStart */
static FSTEntry* FstStart;

/* 80451760-80451764 000C60 0004+00 5/5 0/0 0/0 .sbss            FstStringStart */
static char* FstStringStart;

/* 80451764-80451768 000C64 0004+00 2/2 0/0 0/0 .sbss            MaxEntryNum */
static unsigned int MaxEntryNum;

/* 803484F0-80348528 342E30 0038+00 0/0 2/2 0/0 .text            __DVDFSInit */
void __DVDFSInit() {
    BootInfo = (OSBootInfo*)OSPhysicalToCached(0);
    FstStart = (FSTEntry*)BootInfo->fst_location;

    if (FstStart) {
        MaxEntryNum = FstStart[0].nextEntryOrLength;
        FstStringStart = (char*)&(FstStart[MaxEntryNum]);
    }
}

/* 80451768-8045176C 000C68 0004+00 4/4 0/0 0/0 .sbss            currentDirectory */
static u32 currentDirectory;

/* 80451770-80451778 000C70 0008+00 2/2 3/3 0/0 .sbss            __DVDThreadQueue */
OSThreadQueue __DVDThreadQueue;

/* 8045176C-80451770 000C6C 0004+00 1/1 1/1 0/0 .sbss            __DVDLongFileNameFlag */
u32 __DVDLongFileNameFlag;

/* For convenience */
#define entryIsDir(i) (((FstStart[i].isDirAndStringOff & 0xff000000) == 0) ? FALSE : TRUE)
#define stringOff(i) (FstStart[i].isDirAndStringOff & ~0xff000000)
#define parentDir(i) (FstStart[i].parentOrPosition)
#define nextDir(i) (FstStart[i].nextEntryOrLength)
#define filePosition(i) (FstStart[i].parentOrPosition)
#define fileLength(i) (FstStart[i].nextEntryOrLength)

static BOOL isSame(const char* path, const char* string) {
    while (*string != '\0') {
        if (tolower(*path++) != tolower(*string++)) {
            return FALSE;
        }
    }

    if ((*path == '/') || (*path == '\0')) {
        return TRUE;
    }

    return FALSE;
}

/* 80348528-8034881C 342E68 02F4+00 3/3 6/6 0/0 .text            DVDConvertPathToEntrynum */
int DVDConvertPathToEntrynum(const char* pathPtr) {
    const char* ptr;
    char* stringPtr;
    BOOL isDir;
    u32 length;
    u32 dirLookAt;
    u32 i;
    const char* origPathPtr = pathPtr;
    const char* extentionStart;
    BOOL illegal;
    BOOL extention;

    dirLookAt = currentDirectory;

    while (1) {
        if (*pathPtr == '\0') {
            return (s32)dirLookAt;
        } else if (*pathPtr == '/') {
            dirLookAt = 0;
            pathPtr++;
            continue;
        } else if (*pathPtr == '.') {
            if (*(pathPtr + 1) == '.') {
                if (*(pathPtr + 2) == '/') {
                    dirLookAt = parentDir(dirLookAt);
                    pathPtr += 3;
                    continue;
                } else if (*(pathPtr + 2) == '\0') {
                    return (s32)parentDir(dirLookAt);
                }
            } else if (*(pathPtr + 1) == '/') {
                pathPtr += 2;
                continue;
            } else if (*(pathPtr + 1) == '\0') {
                return (s32)dirLookAt;
            }
        }

        if (__DVDLongFileNameFlag == 0) {
            extention = FALSE;
            illegal = FALSE;

            for (ptr = pathPtr; (*ptr != '\0') && (*ptr != '/'); ptr++) {
                if (*ptr == '.') {
                    if ((ptr - pathPtr > 8) || (extention == TRUE)) {
                        illegal = TRUE;
                        break;
                    }
                    extention = TRUE;
                    extentionStart = ptr + 1;

                } else if (*ptr == ' ')
                    illegal = TRUE;
            }

            if ((extention == TRUE) && (ptr - extentionStart > 3))
                illegal = TRUE;

            if (illegal)
                OSPanic(__FILE__, 387,
                        "DVDConvertEntrynumToPath(possibly DVDOpen or DVDChangeDir or DVDOpenDir): "
                        "specified directory "
                        "or file (%s) doesn't match standard 8.3 format. This is a temporary "
                        "restriction and will be "
                        "removed soon\n",
                        origPathPtr);
        } else {
            for (ptr = pathPtr; (*ptr != '\0') && (*ptr != '/'); ptr++)
                ;
        }

        isDir = (*ptr == '\0') ? FALSE : TRUE;
        length = (u32)(ptr - pathPtr);

        ptr = pathPtr;

        for (i = dirLookAt + 1; i < nextDir(dirLookAt); i = entryIsDir(i) ? nextDir(i) : (i + 1)) {
            if ((entryIsDir(i) == FALSE) && (isDir == TRUE)) {
                continue;
            }

            stringPtr = FstStringStart + stringOff(i);

            if (isSame(ptr, stringPtr) == TRUE) {
                goto next_hier;
            }
        }

        return -1;

    next_hier:
        if (!isDir) {
            return (s32)i;
        }

        dirLookAt = i;
        pathPtr += length + 1;
    }
}

/* 8034881C-80348890 34315C 0074+00 0/0 3/3 0/0 .text            DVDFastOpen */
int DVDFastOpen(s32 entrynum, DVDFileInfo* fileInfo) {
    if ((entrynum < 0) || (entrynum >= MaxEntryNum) || entryIsDir(entrynum)) {
        return FALSE;
    }

    fileInfo->start_address = filePosition(entrynum);
    fileInfo->length = fileLength(entrynum);
    fileInfo->callback = (DVDCallback)NULL;
    fileInfo->block.state = DVD_STATE_END;

    return TRUE;
}

/* 80348890-80348958 3431D0 00C8+00 0/0 3/3 1/1 .text            DVDOpen */
BOOL DVDOpen(const char* fileName, DVDFileInfo* fileInfo) {
    s32 entry;
    char currentDir[128];

    entry = DVDConvertPathToEntrynum(fileName);

    if (0 > entry) {
        DVDGetCurrentDir(currentDir, 128);
        OSReport("Warning: DVDOpen(): file '%s' was not found under %s.\n", fileName, currentDir);
        return FALSE;
    }

    if (entryIsDir(entry)) {
        return FALSE;
    }

    fileInfo->start_address = filePosition(entry);
    fileInfo->length = fileLength(entry);
    fileInfo->callback = (DVDCallback)NULL;
    fileInfo->block.state = DVD_STATE_END;

    return TRUE;
}

/* 80348958-8034897C 343298 0024+00 0/0 4/4 2/2 .text            DVDClose */
BOOL DVDClose(DVDFileInfo* fileInfo) {
    DVDCancel(&(fileInfo->block));
    return TRUE;
}

static u32 myStrncpy(char* dest, char* src, u32 maxlen) {
    u32 i = maxlen;

    while ((i > 0) && (*src != 0)) {
        *dest++ = *src++;
        i--;
    }

    return (maxlen - i);
}

/* 8034897C-80348ADC 3432BC 0160+00 2/2 0/0 0/0 .text            entryToPath */
static u32 entryToPath(u32 entry, char* path, u32 maxlen) {
    char* name;
    u32 loc;

    if (entry == 0) {
        return 0;
    }

    name = FstStringStart + stringOff(entry);

    loc = entryToPath(parentDir(entry), path, maxlen);

    if (loc == maxlen) {
        return loc;
    }

    *(path + loc++) = '/';

    loc += myStrncpy(path + loc, name, maxlen - loc);

    return loc;
}

/* 80348ADC-80348C30 34341C 0154+00 1/1 0/0 0/0 .text            DVDConvertEntrynumToPath */
static BOOL DVDConvertEntrynumToPath(s32 entrynum, char* path, u32 maxlen) {
    u32 loc;

    loc = entryToPath((u32)entrynum, path, maxlen);

    if (loc == maxlen) {
        path[maxlen - 1] = '\0';
        return FALSE;
    }

    if (entryIsDir(entrynum)) {
        if (loc == maxlen - 1) {
            path[loc] = '\0';
            return FALSE;
        }

        path[loc++] = '/';
    }

    path[loc] = '\0';
    return TRUE;
}

/* 80348C30-80348CF4 343570 00C4+00 1/1 0/0 0/0 .text            DVDGetCurrentDir */
BOOL DVDGetCurrentDir(char* path, u32 maxlen) {
    return DVDConvertEntrynumToPath((s32)currentDirectory, path, maxlen);
}

/* 80348CF4-80348D54 343634 0060+00 0/0 2/2 0/0 .text            DVDChangeDir */
BOOL DVDChangeDir(const char* dirName) {
    s32 entry;
    entry = DVDConvertPathToEntrynum(dirName);
    if ((entry < 0) || (entryIsDir(entry) == FALSE)) {
        return FALSE;
    }

    currentDirectory = (u32)entry;

    return TRUE;
}

#define DVD_MIN_TRANSFER_SIZE 32

/* 80348D54-80348E14 343694 00C0+00 0/0 2/2 0/0 .text            DVDReadAsyncPrio */
BOOL DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset,
                      DVDCallback callback, s32 prio) {
    if (!((0 <= offset) && (offset <= fileInfo->length))) {
        OSPanic(__FILE__, 750, "DVDReadAsync(): specified area is out of the file  ");
    }

    if (!((0 <= offset + length) && (offset + length < fileInfo->length + DVD_MIN_TRANSFER_SIZE))) {
        OSPanic(__FILE__, 756, "DVDReadAsync(): specified area is out of the file  ");
    }

    fileInfo->callback = callback;
    DVDReadAbsAsyncPrio(&(fileInfo->block), addr, length, (s32)(fileInfo->start_address + offset),
                        cbForReadAsync, prio);

    return TRUE;
}

#ifndef offsetof
#define offsetof(type, memb) ((u32) & ((type*)0)->memb)
#endif

/* 80348E14-80348E44 343754 0030+00 1/1 0/0 0/0 .text            cbForReadAsync */
static void cbForReadAsync(s32 result, DVDCommandBlock* block) {
    DVDFileInfo* fileInfo;

    fileInfo = (DVDFileInfo*)((char*)block - offsetof(DVDFileInfo, block));
    if (fileInfo->callback) {
        (fileInfo->callback)(result, fileInfo);
    }
}

/* 80348E44-80348F5C 343784 0118+00 0/0 9/9 3/3 .text            DVDReadPrio */
int DVDReadPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, s32 prio) {
    BOOL result;
    DVDCommandBlock* block;
    s32 state;
    BOOL enabled;
    int retVal;

    if (!((0 <= offset) && (offset <= fileInfo->length))) {
        OSPanic(__FILE__, 820, "DVDRead(): specified area is out of the file  ");
    }

    if (!((0 <= offset + length) && (offset + length < fileInfo->length + DVD_MIN_TRANSFER_SIZE))) {
        OSPanic(__FILE__, 826, "DVDRead(): specified area is out of the file  ");
    }

    block = &(fileInfo->block);

    result = DVDReadAbsAsyncPrio(block, addr, length, (s32)(fileInfo->start_address + offset),
                                 cbForReadSync, prio);

    if (result == FALSE) {
        return -1;
    }

    enabled = OSDisableInterrupts();

    while (1) {
        state = ((volatile DVDCommandBlock*)block)->state;

        if (state == DVD_STATE_END) {
            retVal = (s32)block->transferred_size;
            break;
        }
        if (state == DVD_STATE_FATAL_ERROR) {
            retVal = DVD_RESULT_FATAL_ERROR;
            break;
        }
        if (state == DVD_STATE_CANCELED) {
            retVal = DVD_RESULT_CANCELED;
            break;
        }

        OSSleepThread(&__DVDThreadQueue);
    }

    OSRestoreInterrupts(enabled);
    return retVal;
}

/* 80348F5C-80348F80 34389C 0024+00 1/1 0/0 0/0 .text            cbForReadSync */
/* This is based on the revolution SDK, these may not match in all cases */
static void cbForReadSync(s32 result, DVDCommandBlock* block) {
    OSWakeupThread(&__DVDThreadQueue);
}

/* 80348F80-80349040 3438C0 00C0+00 0/0 3/3 0/0 .text            DVDOpenDir */
BOOL DVDOpenDir(const char* dirName, DVDDirectory* dir) {
    s32 entry;
    char currentDir[128];
    entry = DVDConvertPathToEntrynum(dirName);

    if (entry < 0) {
        DVDGetCurrentDir(currentDir, 128);
        OSReport("Warning: DVDOpenDir(): file '%s' was not found under %s.\n", dirName, currentDir);
        return FALSE;
    }

    if (!entryIsDir(entry)) {
        return FALSE;
    }

    dir->entry_number = (u32)entry;
    dir->location = (u32)entry + 1;
    dir->next = nextDir(entry);

    return TRUE;
}

/* 80349040-803490E4 343980 00A4+00 0/0 3/3 0/0 .text            DVDReadDir */
BOOL DVDReadDir(DVDDirectory* dir, DVDDirectoryEntry* dirent) {
    u32 loc = dir->location;
    if ((loc <= dir->entry_number) || (dir->next <= loc)) {
        return FALSE;
    }

    dirent->entry_number = loc;
    dirent->is_directory = entryIsDir(loc);
    dirent->name = FstStringStart + stringOff(loc);

    dir->location = entryIsDir(loc) ? nextDir(loc) : (loc + 1);

    return TRUE;
}

/* 803490E4-803490EC 343A24 0008+00 0/0 3/3 0/0 .text            DVDCloseDir */
BOOL DVDCloseDir() {
    return 1;
}