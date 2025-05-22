#include "metrotrk/mainloop.h"
#include "metrotrk/dispatch.h"
#include "metrotrk/msgbuf.h"
#include "metrotrk/nubevent.h"
#include "metrotrk/serpoll.h"
#include "metrotrk/targimpl.h"

void TRKHandleRequestEvent(NubEvent* ev) { TRKDispatchMessage(TRKGetBuffer(ev->fMessageBufferID)); }

void TRKHandleSupportEvent(NubEvent* ev) { TRKTargetSupportRequest(); }

void TRKIdle(void) {
    if (!TRKTargetStopped()) {
        TRKTargetContinue();
    }
}

void TRKNubMainLoop(void) {
    NubEvent event;
    BOOL var_r31 = FALSE;
    BOOL var_r30 = FALSE;

    while (!var_r31) {
        if (TRKGetNextEvent(&event) != FALSE) {
            var_r30 = FALSE;
            switch (event.fType) {
                case kNullEvent:
                    break;
                case kRequestEvent:
                    TRKHandleRequestEvent(&event);
                    break;
                case kShutdownEvent:
                    var_r31 = TRUE;
                    break;
                case kBreakpointEvent:
                case kExceptionEvent:
                    TRKTargetInterrupt(&event);
                    break;
                case kSupportEvent:
                    TRKHandleSupportEvent(&event);
                    break;
            }
            TRKDestructEvent(&event);
        } else if (!var_r30 || *(u8*)gTRKInputPendingPtr != 0) {
            var_r30 = TRUE;
            TRKGetInput();
        } else {
            TRKIdle();
            var_r30 = FALSE;
        }
    }
}
