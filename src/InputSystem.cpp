#include "game/InputSystem.h"
#include "dolphin/pad.h"
#include "dolphin/os.h"
#include "game/macros.h"

InputSystem gInputSystem;

InputSystem::InputSystem() {
    m_debounceTicks = (OS_BUS_CLOCK / 4000) * 50; // @BUG: This is too fast.
}

InputSystem::~InputSystem() {}

bool InputSystem::DebounceTimerExpired(int contId, int joyId, int dir) {
    OSTick *pDirection;
    OSTick dirTick;
    OSTick currentTick;

    DEBUGASSERTLINE(142, contId >= 0 && contId < PAD_MAX_CONTROLLERS);
    DEBUGASSERTLINE(143, joyId == NGPS_JOY_LEFT || joyId == NGPS_JOY_RIGHT);
    DEBUGASSERTLINE(144, dir >= 0 && dir < DIR_COUNT);

    if (contId > m_debugControllerId) {
        return false;
    }

    pDirection = m_InputTicks[contId].Stick[joyId].Direction;
    dirTick   = pDirection[dir];
    if (dirTick != 0) {
        currentTick = OSGetTick();
        if (dirTick != m_tick && ElapsedTicks(currentTick, dirTick) < m_debounceTicks) {
            goto not_expired;
        }
        if (dirTick != m_tick) {
            pDirection[dir] = 0;
        }
        return true;
    }
not_expired:
    return false;
}

void InputSystem::GetJoystickVector(int contId, int joyId, float *x, float *y, int mode) {
    DEBUGASSERTLINE(254, contId >= 0 && contId < PAD_MAX_CONTROLLERS);
    DEBUGASSERTLINE(255, joyId == NGPS_JOY_LEFT || joyId == NGPS_JOY_RIGHT);

    
}

u32 InputSystem::ElapsedTicks(OSTick currentTick, OSTick savedTick) {
    if (currentTick < savedTick) {
        return (-1 - savedTick) + currentTick;
    }
    return currentTick - savedTick;
}
