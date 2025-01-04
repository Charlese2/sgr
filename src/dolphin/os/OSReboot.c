#include "dolphin.h"
#include "dolphin/os.h"
#include "dolphin/os/OSReboot.h"
#include "dvd/__dvd.h"
#include "os/__os.h"

static void * SaveStart;
static void * SaveEnd;

static volatile int Prepared;

#define OS_BOOTROM_ADDR         0x81300000

extern void * unk AT_ADDRESS(0x817ffff8);
extern u32 OS_RESET_CODE AT_ADDRESS(0x817ffffc);
extern u8 OS_REBOOT_BOOL AT_ADDRESS(0x800030E2);

static asm void Run(void * entryPoint) {
    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    or r31, r3, r3
    bl OSDisableInterrupts;
    bl ICFlashInvalidate;
    sync
    isync
    mtlr r31
    blr
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
}

static void ReadApploader(void * addr, s32 length, s32 offset) {
    DVDCommandBlock block;
    while (!Prepared) {}
    DVDReadAbsAsyncForBS( &block, addr, length, offset + 0x2440, NULL);
    while (TRUE) {
        switch (block.state) {
        case 0:
            return;
        case 1:
            break;
        case -1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            __OSDoHotReset(OS_RESET_CODE);
            continue;
        default:
            break;
        }
    }
}

static void Callback() {
    Prepared = 1;
}

void __OSReboot(u32 resetCode, int forceMenu) {
    u32 offset;
    u32 length;
    OSContext exceptionContext;

    OSDisableInterrupts();
    OS_RESET_CODE = 0;
    unk = 0;
    OS_REBOOT_BOOL = TRUE;
    BOOT_REGION_START = (u32)SaveStart;
    BOOT_REGION_END = (u32)SaveEnd;

    OSClearContext(&exceptionContext);
    OSSetCurrentContext(&exceptionContext);
    DVDInit();
    DVDSetAutoInvalidation(TRUE);
    __DVDPrepareResetAsync(Callback);
    if(!DVDCheckDisk()) {
        __OSDoHotReset(OS_RESET_CODE);
    }
    __OSMaskInterrupts(0xffffffe0);
    __OSUnmaskInterrupts(0x400);
    OSEnableInterrupts();
    
    ReadApploader(&Header, 0x20, 0);
    offset = Header.size + 32;
    length = OSRoundUp32B(Header.rebootSize);
    ReadApploader((void *)OS_BOOTROM_ADDR, length, offset);
    ICInvalidateRange((void *)OS_BOOTROM_ADDR, length);
    Run((void *)OS_BOOTROM_ADDR);
}

void OSSetSaveRegion(void * start, void * end) {

}

void OSGetSaveRegion(void * start, void * end) {
    
}
