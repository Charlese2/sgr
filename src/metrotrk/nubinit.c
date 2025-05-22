#include "metrotrk/nubinit.h"
#include "metrotrk/dispatch.h"
#include "metrotrk/dolphin_trk_glue.h"
#include "metrotrk/msgbuf.h"
#include "metrotrk/serpoll.h"
#include "metrotrk/targimpl.h"
#include "metrotrk/usr_put.h"

BOOL gTRKBigEndian;

DSError TRKInitializeNub(void) {
    DSError result;
    DSError resultTemp;

    result = TRKInitializeEndian();

    if (result == kNoError) {
        usr_put_initialize();
    }

    if (result == kNoError) {
        result = TRKInitializeEventQueue();
    }

    if (result == kNoError) {
        result = TRKInitializeMessageBuffers();
    }

    // v0.1 only
    if (result == kNoError) {
        result = TRKInitializeDispatcher();
    }

    if (result == kNoError) {
        // 0xE100 only on v0.1
        resultTemp = TRKInitializeIntDrivenUART(0xE100, 1, 0, &gTRKInputPendingPtr);
        TRKTargetSetInputPendingPtr(gTRKInputPendingPtr);
        if (resultTemp != kNoError) {
            result = resultTemp;
        }
    }

    if (result == kNoError) {
        result = TRKInitializeSerialHandler();
    }

    if (result == kNoError) {
        result = TRKInitializeTarget();
    }

    return result;
}

DSError TRKTerminateNub(void) {
    TRKTerminateSerialHandler();
    return kNoError;
}

void TRKNubWelcome(void) { TRK_board_display("MetroTRK for GAMECUBE v0.10"); }

BOOL TRKInitializeEndian(void) {
    u8 bendian[4];
    BOOL result = FALSE;

    gTRKBigEndian = TRUE;

    bendian[0] = 0x12;
    bendian[1] = 0x34;
    bendian[2] = 0x56;
    bendian[3] = 0x78;

    if (*(u32*)bendian == 0x12345678) {
        gTRKBigEndian = TRUE;
    } else if (*(u32*)bendian == 0x78563412) {
        gTRKBigEndian = FALSE;
    } else {
        result = TRUE;
    }

    return result;
}
