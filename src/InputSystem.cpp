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
    OSTick currentTick;
    u32 savedTick;
    u32 elapsedTicks;

    DEBUGASSERTLINE(142, contId >= 0 && contId < PAD_MAX_CONTROLLERS);
    DEBUGASSERTLINE(143, joyId == NGPS_JOY_LEFT || joyId == NGPS_JOY_RIGHT);
    DEBUGASSERTLINE(144, dir >= 0 && dir < DIR_COUNT);
    
    savedTick = m_InputTicks[contId].Stick[joyId].Direction[dir];
    if (savedTick == 0 || (currentTick = OSGetTick()), savedTick != m_tick && (elapsedTicks = ElapsedTicks(currentTick, savedTick)),
        elapsedTicks < m_debounceTicks) {
        return FALSE;
    } else {
        if (savedTick != m_tick) {
            m_InputTicks[contId].Stick[joyId].Direction[dir] = 0;
        }
        return TRUE;
    }
}

void InputSystem::GetJoystickVector(int contId, int joyId, float *x, float *y, int mode) {}

u32 InputSystem::ElapsedTicks(OSTick currentTick, OSTick savedTick) {
    if (currentTick < savedTick) {
        return (-1 - savedTick) + currentTick;
    }
    return currentTick - savedTick;
}
