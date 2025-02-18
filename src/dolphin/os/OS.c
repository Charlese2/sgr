#include "dolphin/base/PPCArch.h"
#include "dolphin/os/OSAlarm.h"
#include "dolphin/os.h"
#include <dolphin.h>
#include <dolphin/exi.h>
#include <dolphin/os.h>
#include <dolphin/db.h>
#include <macros.h>

void EnableMetroTRKInterrupts(void);

// internal headers
#include "__os.h"

#define OS_BI2_DEBUG_ADDRESS 0x800000F4
#define DEBUGFLAG_ADDR 0x800030E8
#define OS_DEBUG_ADDRESS_2 0x800030E9
#define OS_CURRENTCONTEXT_PADDR 0x00C0

#define OS_EXCEPTIONTABLE_ADDR 0x3000
#define OS_DBJUMPPOINT_ADDR 0x60
// memory locations for important stuff
#define OS_CACHED_REGION_PREFIX 0x8000
#define OS_BI2_DEBUG_ADDRESS 0x800000F4
#define OS_BI2_DEBUGFLAG_OFFSET 0xC
#define PAD3_BUTTON_ADDR 0x800030E4
#define OS_DVD_DEVICECODE 0x800030E6
#define DEBUGFLAG_ADDR 0x800030E8
#define OS_DEBUG_ADDRESS_2 0x800030E9
#define DB_EXCEPTIONRET_OFFSET 0xC
#define DB_EXCEPTIONDEST_OFFSET 0x8

extern unsigned long __DVDLongFileNameFlag;
extern unsigned long __PADSpec;
extern unsigned char __ArenaLo[];
extern char _stack_addr[];
extern unsigned char __ArenaHi[];

// dummy entry points to the OS Exception vector
void __OSEVStart(void);
void __OSEVEnd(void);
void __OSEVSetNumber(void);
void __OSExceptionVector(void);

void __DBVECTOR(void);
void __OSDBINTSTART(void);
void __OSDBINTEND(void);
void __OSDBJUMPSTART(void);
void __OSDBJUMPEND(void);

#define NOP 0x60000000

static struct OSBootInfo_s * BootInfo;
static u32 * BI2DebugFlag;
static u32* BI2DebugFlagHolder;
static BOOL __OSIsGcam;
static double ZeroF;
static int AreWeInitialized;
static void (* * OSExceptionTable)(unsigned char, struct OSContext *);
OSTime __OSStartTime;
BOOL __OSInIPL;

// functions
static asm void __OSInitFPRs(void);
static void OSExceptionInit(void);
static void OSDefaultExceptionHandler(unsigned char exception /* r3 */, struct OSContext * context /* r4 */);

unsigned long __OSIsDebuggerPresent() {
    return *(u32*)OSPhysicalToCached(0x40);
}

/* clang-format off */
static asm void __OSInitFPRs(void)
{
    nofralloc
    lfd     fp0, ZeroF
    fmr     fp1, fp0
    fmr     fp2, fp0
    fmr     fp3, fp0
    fmr     fp4, fp0
    fmr     fp5, fp0
    fmr     fp6, fp0
    fmr     fp7, fp0
    fmr     fp8, fp0
    fmr     fp9, fp0
    fmr     fp10, fp0
    fmr     fp11, fp0
    fmr     fp12, fp0
    fmr     fp13, fp0
    fmr     fp14, fp0
    fmr     fp15, fp0
    fmr     fp16, fp0
    fmr     fp17, fp0
    fmr     fp18, fp0
    fmr     fp19, fp0
    fmr     fp20, fp0
    fmr     fp21, fp0
    fmr     fp22, fp0
    fmr     fp23, fp0
    fmr     fp24, fp0
    fmr     fp25, fp0
    fmr     fp26, fp0
    fmr     fp27, fp0
    fmr     fp28, fp0
    fmr     fp29, fp0
    fmr     fp30, fp0
    fmr     fp31, fp0
    blr
}

unsigned long OSGetConsoleType() {
    if ((!BootInfo) || (BootInfo->consoleType == 0)) {
        return OS_CONSOLE_ARTHUR;
    }
    return BootInfo->consoleType;
}

void* __OSSavedRegionStart;
void* __OSSavedRegionEnd;

static void ClearArena(void) {
    if (OSGetResetCode() != 0x80000000) {
        __OSSavedRegionStart = 0;
        __OSSavedRegionEnd = 0;
        memset(OSGetArenaLo(), 0U, (u32)OSGetArenaHi() - (u32)OSGetArenaLo());
        return;
    }

    __OSSavedRegionStart = (void*)BOOT_REGION_START;
    __OSSavedRegionEnd = (void*)BOOT_REGION_END;
    if (*(u32*)&BOOT_REGION_START == 0U) {
        memset(OSGetArenaLo(), 0U, (u32)OSGetArenaHi() - (u32)OSGetArenaLo());
        return;
    }

    if ((u32)OSGetArenaLo() < *(u32*)&__OSSavedRegionStart) {
        if ((u32)OSGetArenaHi() <= *(u32*)&__OSSavedRegionStart) {
            memset(OSGetArenaLo(), 0U, (u32)OSGetArenaHi() - (u32)OSGetArenaLo());
            return;
        }

        memset(OSGetArenaLo(), 0U, *(u32*)&__OSSavedRegionStart - (u32)OSGetArenaLo());

        if ((u32)OSGetArenaHi() > *(u32*)&__OSSavedRegionEnd) {
            memset(__OSSavedRegionEnd, 0,
                   (u32)OSGetArenaHi() - (u32)__OSSavedRegionEnd);
        }
    }
}

static DVDDriveInfo DriveInfo;

static void InquiryCallback(s32 result, DVDCommandBlock* block) {
    switch (block->state) {
    case 0:
        __OSDeviceCode = (u16)(0x8000 | DriveInfo.deviceCode);
        break;
    default:
        __OSDeviceCode = 1;
        break;
    }
}

static u8 DriveBlock[48];

void OSInit() {
    BI2Debug* DebugInfo = NULL;
    u32 consoleType;

    if (AreWeInitialized == 0) {
        AreWeInitialized = 1;

        __OSStartTime = __OSGetSystemTime();
        OSDisableInterrupts();


        PPCDisableSpeculation();
        PPCSetFpNonIEEEMode();

        BI2DebugFlag = NULL;
        BootInfo = (struct OSBootInfo_s *)OSPhysicalToCached(0);

        __DVDLongFileNameFlag = 0;
        
        DebugInfo = (BI2Debug*)*(u32*)OS_BI2_DEBUG_ADDRESS;


        if (DebugInfo != NULL) {
            BI2DebugFlag = &DebugInfo->debugFlag;
            __PADSpec = (u32)DebugInfo->padSpec;
            *((u8*)DEBUGFLAG_ADDR) = (u8)*BI2DebugFlag;
            *((u8*)OS_DEBUG_ADDRESS_2) = (u8)__PADSpec;
        } else if (BootInfo->arenaHi) {
            BI2DebugFlagHolder =
                (u32*)*((u8*)DEBUGFLAG_ADDR);
            BI2DebugFlag = (u32*)&BI2DebugFlagHolder;
            __PADSpec = (u32) * ((u8*)OS_DEBUG_ADDRESS_2);
        }

        __DVDLongFileNameFlag = 1;
        OSSetArenaLo((!BootInfo->arenaLo) ? &__ArenaLo : BootInfo->arenaLo);
        if ((!BootInfo->arenaLo) && (BI2DebugFlag) && (*(u32*)BI2DebugFlag < 2)) {
            OSSetArenaLo((void*)(((u32)(char*)&_stack_addr + 0x1F) & 0xFFFFFFE0));
        }
        OSSetArenaHi((!BootInfo->arenaHi) ? &__ArenaHi : BootInfo->arenaHi);
        OSExceptionInit();
        __OSInitSystemCall();
        OSInitAlarm();
        __OSModuleInit();
        __OSInterruptInit();
        __OSSetInterruptHandler(0x16, &__OSResetSWInterruptHandler);
        __OSContextInit();
        __OSCacheInit();
        EXIInit();
        SIInit();
        __OSInitSram();
        __OSThreadInit();
        __OSInitAudioSystem();
        PPCMthid2(PPCMfhid2() & 0xbfffffff);
        ASSERTLINE(0x252, BootInfo); // oh sure, assert NOW, you've already dereferenced it a bunch of times.
        if ((BootInfo->consoleType & OS_CONSOLE_DEVELOPMENT) != 0) {
            BootInfo->consoleType = OS_CONSOLE_DEVHW1;
        } else {
            BootInfo->consoleType = OS_CONSOLE_RETAIL1;
        }

        BootInfo->consoleType += (__PIRegs[11] & 0xF0000000) >> 28;

        if (!__OSInIPL) {
            __OSInitMemoryProtection();
        }

        OSReport("\nDolphin OS $Revision: 54 $.\n");
        OSReport("Kernel built : %s %s\n", "Jun  5 2002", "02:09:12");
        OSReport("Console Type : ");



        // work out what console type this corresponds to and report it
        // consoleTypeSwitchHi = inputConsoleType & 0xF0000000;
        consoleType = OSGetConsoleType();
        if ((consoleType & 0x10000000) == OS_CONSOLE_RETAIL) { // check "first" byte
            OSReport("Retail %d\n", consoleType);
        } else {
            switch (consoleType) { // if "first" byte is 2, check "the rest"
                case OS_CONSOLE_EMULATOR:
                    OSReport("Mac Emulator\n");
                    break;
                case OS_CONSOLE_PC_EMULATOR:
                    OSReport("PC Emulator\n");
                    break;
                case OS_CONSOLE_ARTHUR:
                    OSReport("EPPC Arthur\n");
                    break;
                case OS_CONSOLE_MINNOW:
                    OSReport("EPPC Minnow\n");
                    break;
                default:
                    OSReport("Development HW%d\n", ((u32)consoleType - 0x10000000) - 3);
                    break;
              }
        }
        // report memory size
        OSReport("Memory %d MB\n", (u32)BootInfo->memorySize >> 0x14U);
        // report heap bounds
        OSReport("Arena : 0x%x - 0x%x\n", OSGetArenaLo(), OSGetArenaHi());

        

        // if location of debug flag exists, and flag is >= 2, enable MetroTRKInterrupts
        if (BI2DebugFlag && ((*BI2DebugFlag) >= 2)) {
            EnableMetroTRKInterrupts();
        }
        ClearArena();
        OSEnableInterrupts();

        if (__OSInIPL == FALSE) {
            DVDInit();
            if (__OSIsGcam) {
                __OSDeviceCode = 0x9000;
                return;
            }
        DCInvalidateRange(&DriveInfo, sizeof(DriveInfo));
        DVDInquiryAsync((DVDCommandBlock*)&DriveBlock, &DriveInfo, InquiryCallback);
        }
    }
}

static u32 __OSExceptionLocations[] = {
    0x00000100,  // 0  System reset
    0x00000200,  // 1  Machine check
    0x00000300,  // 2  DSI - seg fault or DABR
    0x00000400,  // 3  ISI
    0x00000500,  // 4  External interrupt
    0x00000600,  // 5  Alignment
    0x00000700,  // 6  Program
    0x00000800,  // 7  FP Unavailable
    0x00000900,  // 8  Decrementer
    0x00000C00,  // 9  System call
    0x00000D00,  // 10 Trace
    0x00000F00,  // 11 Performance monitor
    0x00001300,  // 12 Instruction address breakpoint.
    0x00001400,  // 13 System management interrupt
    0x00001700   // 14 Thermal interrupt
};

#if DEBUG
char * __OSExceptionNames[15] = {
	"System reset",
	"MachineCheck",
	"DSI",
	"ISI",
	"External Int.",
	"Alignment",
	"Program",
	"FP Unavailable",
	"Decrementer",
	"System call",
	"Trace",
	"Perf mon",
	"IABR",
	"SMI",
	"Thermal Int.",
};
#endif

static void OSExceptionInit(void) {
    __OSException exception;
    void* destAddr;
    
    // These two vars help us change the exception number embedded
    // in the exception handler code.
    u32* opCodeAddr;
    u32 oldOpCode;
    
    // Address range of the actual code to be copied.
    u8* handlerStart;
    u32 handlerSize;
    
    ASSERTMSGLINE(0x2F1, ((u32)&__OSEVEnd - (u32)&__OSEVStart) <= 0x100, "OSExceptionInit(): too big exception vector code.");
      
    // Install the first level exception vector.
    opCodeAddr = (u32*)__OSEVSetNumber;
    oldOpCode = *opCodeAddr;
    handlerStart = (u8*)__OSEVStart;
    handlerSize = (u32)((u8*)__OSEVEnd - (u8*)__OSEVStart);
    
    // Install the DB integrator, only if we are the first OSInit to be run
    destAddr = (void*)OSPhysicalToCached(OS_DBJUMPPOINT_ADDR);
    if (*(u32*)destAddr == 0) // Lomem should be zero cleared only once by BS2
    {
        DBPrintf("Installing OSDBIntegrator\n");
        memcpy(destAddr, (void*)__OSDBINTSTART, (u32)__OSDBINTEND - (u32)__OSDBINTSTART);
        DCFlushRangeNoSync(destAddr, (u32)__OSDBINTEND - (u32)__OSDBINTSTART);
        __sync();
        ICInvalidateRange(destAddr, (u32)__OSDBINTEND - (u32)__OSDBINTSTART);
    }
    
    // Copy the right vector into the table
    for (exception = 0; exception < __OS_EXCEPTION_MAX; exception++) {
        if (BI2DebugFlag && (*BI2DebugFlag >= 2) && __DBIsExceptionMarked(exception)) {
            // this DBPrintf is suspicious.
            DBPrintf(">>> OSINIT: exception %d commandeered by TRK\n", exception);
            continue;
        }
        
        // Modify the copy of code in text before transferring
        // to the exception table.
        *opCodeAddr = oldOpCode | exception;
        
        // Modify opcodes at __DBVECTOR if necessary
        if (__DBIsExceptionMarked(exception)) {
            DBPrintf(">>> OSINIT: exception %d vectored to debugger\n", exception);
            memcpy((void*)__DBVECTOR, (void*)__OSDBINTEND, (u32)__OSDBJUMPEND - (u32)__OSDBINTEND);
        } else {
            // make sure the opcodes are still nop
            u32* ops = (u32*)__DBVECTOR;
            int cb;
            
            for (cb = 0; cb < (u32)__OSDBJUMPEND - (u32)__OSDBINTEND; cb += sizeof(u32)) {
                *ops++ = NOP;
            }
        }
        
        // Install the modified handler.
        destAddr = (void*)OSPhysicalToCached(__OSExceptionLocations[(u32)exception]);
        memcpy(destAddr, handlerStart, handlerSize);
        DCFlushRangeNoSync(destAddr, handlerSize);
        __sync();
        ICInvalidateRange(destAddr, handlerSize);
    }
    // initialize pointer to exception table
    OSExceptionTable = (void*)OSPhysicalToCached(OS_EXCEPTIONTABLE_ADDR);
    
    // install default exception handlers
    for (exception = 0; exception < __OS_EXCEPTION_MAX; exception++) {
        __OSSetExceptionHandler(exception, OSDefaultExceptionHandler);
    }
    
    // restore the old opcode, so that we can re-start an application without
    // downloading the text segments
    *opCodeAddr = oldOpCode;
    
    DBPrintf("Exceptions initialized...\n");
}

static asm void __OSDBIntegrator(void) {
  /* clang-format off */
    nofralloc
entry __OSDBINTSTART
    li      r5, OS_DBINTERFACE_ADDR
    mflr    r3
    stw     r3, DB_EXCEPTIONRET_OFFSET(r5)
    lwz     r3, DB_EXCEPTIONDEST_OFFSET(r5)
    oris    r3, r3, OS_CACHED_REGION_PREFIX
    mtlr    r3
    li      r3, 0x30 // MSR_IR | MSR_DR     // turn on memory addressing
    mtmsr   r3
    blr
entry __OSDBINTEND
  /* clang-format on */
}

static asm void __OSDBJump(void){
    /* clang-format off */

    nofralloc
entry __OSDBJUMPSTART
    bla     OS_DBJUMPPOINT_ADDR
entry __OSDBJUMPEND
    /* clang-format on */

} 

__OSExceptionHandler __OSSetExceptionHandler(__OSException exception, __OSExceptionHandler handler) {
    __OSExceptionHandler oldHandler;
    
    ASSERTMSGLINE(0x37F, exception < __OS_EXCEPTION_MAX, "__OSSetExceptionHandler(): unknown exception."); 
    
    oldHandler = OSExceptionTable[exception];
    OSExceptionTable[exception] = handler;
    return oldHandler;
}

__OSExceptionHandler __OSGetExceptionHandler(__OSException exception) {
    ASSERTMSGLINE(0x396, exception < __OS_EXCEPTION_MAX, "__OSGetExceptionHandler(): unknown exception.");
    return OSExceptionTable[exception];
}

static asm void OSExceptionVector(void) {
  /* clang-format off */
    nofralloc

entry __OSEVStart
    // Save r4 into SPRG0
    mtsprg  0, r4

    // Load current context physical address into r4
    lwz     r4, OS_CURRENTCONTEXT_PADDR

    // Save r3 - r5 into the current context
    stw     r3, OS_CONTEXT_R3(r4)
    mfsprg  r3, 0
    stw     r3, OS_CONTEXT_R4(r4)
    stw     r5, OS_CONTEXT_R5(r4)

    lhz     r3, OS_CONTEXT_STATE(r4)
    ori     r3, r3, OS_CONTEXT_STATE_EXC
    sth     r3, OS_CONTEXT_STATE(r4)

    // Save misc registers
    mfcr    r3
    stw     r3, OS_CONTEXT_CR(r4)
    mflr    r3
    stw     r3, OS_CONTEXT_LR(r4)
    mfctr   r3
    stw     r3, OS_CONTEXT_CTR(r4)
    mfxer   r3
    stw     r3, OS_CONTEXT_XER(r4)
    mfsrr0  r3
    stw     r3, OS_CONTEXT_SRR0(r4)
    mfsrr1  r3
    stw     r3, OS_CONTEXT_SRR1(r4)
    mr      r5, r3

entry __DBVECTOR
    nop

    // Set SRR1[IR|DR] to turn on address
    // translation at the next RFI
    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3

    // This lets us change the exception number based on the
    // exception we're installing.
entry __OSEVSetNumber
    addi    r3, 0, 0x0000

    // Load current context virtual address into r4
    lwz     r4, 0xD4

    // Check non-recoverable interrupt
    rlwinm. r5, r5, 0, MSR_RI_BIT, MSR_RI_BIT
    bne     recoverable
    addis   r5, 0,  OSDefaultExceptionHandler@ha
    addi    r5, r5, OSDefaultExceptionHandler@l
    mtsrr0  r5
    rfi
    // NOT REACHED HERE

recoverable:
    // Locate exception handler.
    rlwinm  r5, r3, 2, 22, 29               // r5 contains exception*4
    lwz     r5, OS_EXCEPTIONTABLE_ADDR(r5)
    mtsrr0  r5

    // Final state
    // r3 - exception number
    // r4 - pointer to context
    // r5 - garbage
    // srr0 - exception handler
    // srr1 - address translation enalbed, not yet recoverable

    rfi
    // NOT REACHED HERE
    // The handler will restore state

entry __OSEVEnd
    nop
  /* clang-format on */
}

void __OSUnhandledException(__OSException exception, OSContext* context, u32 dsisr, u32 dar);
asm void OSDefaultExceptionHandler(register __OSException exception, register OSContext* context) {
  /* clang-format off */
    nofralloc
    OS_EXCEPTION_SAVE_GPRS(context)
    mfdsisr r5
    mfdar   r6
    stwu r1, -0x8(r1)
    b       __OSUnhandledException
    /* clang-foramt on */
}

void __OSPSInit(void)
{
	PPCMthid2(PPCMfhid2() | 0x80000000 | 0x20000000);
	ICFlashInvalidate();
	__sync();
	// clang-format off
    asm
    {
        li      r3, 0
        mtspr   GQR0, r3
    }
  // clang-format on
}

u32 __OSGetDIConfig() {
    return __DIRegs[9] & 0xFF;
}
