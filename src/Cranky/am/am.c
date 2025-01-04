#include "dolphin/am.h"
#include "dolphin/ar.h"
#include "am/__am.h"

static u32 __AMPendingReads;
static u32 __AMFreeBytes;
static u32 __AMStackLocation;
static u32 __AMStackPointer[MAX_STACKS];

static void __AM_dvd_callback(s32 result, struct DVDFileInfo * handle) {

}

static void __AM_arq_callback(u32 task) {

}

static void __AM_arq_poll_callback(u32 task) {
    u32 i;
    for (i = 0; i < MAX_STACKS; i++) {
        if((ARQRequest*)task == &__AMReadInfo[i].arq_handle) {
            task = (u32)(&__AMReadInfo[i].file_handle);
            break;
        }
    }

    __AMReadInfo[i].poll_flag = 1;
}

void * AMLoadFile(char * path, u32 * length) {

}

u32 AMPush(char * path) {

}

u32 AMPushData(void * buffer, u32 length) {

}

u32 __AMPushBuffered(char * path, void * buffer, u32 offset, u32 buffer_size, void (* callback)(char *), int async_flag) {
    s32 enabled;
    u32 round_file_length;
    u32 stack_index = 0;
    u32 remainder = 0;
    u32 read_request_length;
    s32 actual_read_length;

    if (__AMStackLocation < 0xff) {
        if(DVDOpen(path, &__AMReadInfo[stack_index].file_handle)) {
            if (__AMFreeBytes < __AMReadInfo[stack_index].file_length) {
                OSReport("AMPushBuffered(): WARNING: Not enough space in ARAM.\n");

            }
        } else {
            char buffer[64];
            sprintf(buffer,"AMPushBuffered(): Unable to open file '%s'\n");
            OSReport(buffer, path);
            OSPanic("am.c", 0x1ee, "AM: FATAL ERROR\n");
        }
    }


    enabled = OSDisableInterrupts();
    OSRestoreInterrupts(enabled);

    DVDReadAsyncPrio(&__AMReadInfo[stack_index].file_handle, buffer, buffer_size, remainder, (DVDCallback)callback, FALSE);

    return stack_index;
}

void AMPop() {

}

u32 AMGetZeroBuffer() {
    return __AMStackPointer[0];
}

u32 AMGetReadStatus() {

}

u32 AMGetFreeSize() {

}

u32 AMGetStackPointer() {

}

void AMInit(u32 aramBase, u32 aramBytes) {

}
