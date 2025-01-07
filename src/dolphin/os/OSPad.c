#include "__os.h"
#include <dolphin/exi.h>
#include <dolphin/os.h>
#include <macros.h>

unsigned short OSGetWirelessID(int padNumber){
    struct OSSramEx * sramEx = __OSLockSramEx();
    short wirelessPadId;

    wirelessPadId = sramEx->wirelessPadID[padNumber];
    UnlockSram(0,20);
    return wirelessPadId;
}

void OSSetWirelessID(int padNumber, unsigned short ID){
    struct OSSramEx * sramEx = __OSLockSramEx();

    sramEx->wirelessPadID[padNumber] = ID;
    if (ID != sramEx->wirelessPadID[padNumber]) {
        UnlockSram(1, 20);
    }
    UnlockSram(0, 20);
}
