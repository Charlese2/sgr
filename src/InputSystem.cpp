#include "game/InputSystem.h"
#include "game/new_math.h"
#include "dolphin/pad.h"
#include "dolphin/os.h"
#include "game/macros.h"

InputSystem gInputSystem;

InputSystem::InputSystem() {
    m_DebugControllerId = 0;
    m_DebounceTicks     = OSMillisecondsToTicks(50); // @BUG: This is too fast.
}

InputSystem::~InputSystem() {}

u32 InputSystem::IsButtonPressed(int contId, int ButtonId) {
    float leftTriggerInput;
    float rightTriggerInput;

#ifndef DEBUG
    if (contId > m_DebugControllerId) {
        return false;
    }
#endif

    if (contId == CONTROLLER_ONE && ButtonId == 10) {
        leftTriggerInput = GetAnalogTriggerInput(contId, kAnalogTriggerLeft);
        if (leftTriggerInput > 0.5f) {
            if (m_LeftTriggerDown == 0) {
                m_LeftTriggerDown = 1;
                return 1;
            }
        } else if (leftTriggerInput < 0.15f) {
            m_LeftTriggerDown = 0;
        }
        return 0;
    }
    if (contId == CONTROLLER_ONE && ButtonId == 11) {
        rightTriggerInput = GetAnalogTriggerInput(contId, kAnalogTriggerRight);
        if (rightTriggerInput > 0.5f) {
            if (m_RightTriggerDown == 0) {
                m_RightTriggerDown = 1;
                return 1;
            }
        } else if (rightTriggerInput < 0.15f) {
            m_RightTriggerDown = 0;
        }
        return 0;
    }
    return CrankyInput::ButtonPressed(contId, buttons[ButtonId], true);
}

bool InputSystem::DebounceTimerExpired(int contId, int joyId, int dir) {
    OSTick *pDirection;
    OSTick dirTick;
    OSTick currentTick;

    DEBUGASSERTLINE(142, contId >= 0 && contId < PAD_MAX_CONTROLLERS);
    DEBUGASSERTLINE(143, joyId == NGPS_JOY_LEFT || joyId == NGPS_JOY_RIGHT);
    DEBUGASSERTLINE(144, dir >= 0 && dir < DIR_COUNT);

#ifndef DEBUG
    if (contId > m_DebugControllerId) {
        return false;
    }
#endif
    pDirection = m_InputTicks[contId].Stick[joyId].Direction;
    dirTick    = pDirection[dir];
    if (dirTick != 0) {
        currentTick = OSGetTick();
        if (dirTick != m_Tick && ElapsedTicks(currentTick, dirTick) < m_DebounceTicks) {
            goto not_expired;
        }
        if (dirTick != m_Tick) {
            pDirection[dir] = 0;
        }
        return true;
    }
not_expired:
    return false;
}

bool InputSystem::IsInDeadzone(int contId, int joyId) {
    float x, y;
    PADStatus *pPadStatus;

    DEBUGASSERTLINE(179, contId >= 0 && contId < PAD_MAX_CONTROLLERS);
    DEBUGASSERTLINE(180, joyId == NGPS_JOY_LEFT || joyId == NGPS_JOY_RIGHT);

#ifndef DEBUG
    if (contId > m_DebugControllerId) {
        return false;
    }
#endif
    if (m_JoystickLow <= 0.0f) {
        return false;
    }
    pPadStatus = &m_PadStatus[contId];
    if (joyId == 0) {
        x = fl_abs(pPadStatus->stickX);
        y = fl_abs(pPadStatus->stickY);
    } else {
        x = fl_abs(pPadStatus->substickX);
        y = fl_abs(pPadStatus->substickY);
    }
    if (x <= m_JoystickLow && y <= m_JoystickLow) {
        return true;
    }
    return false;
}

bool InputSystem::IsNoButtonPressed(int contId) {
    DEBUGASSERTLINE(219, contId >= 0 && contId < PAD_MAX_CONTROLLERS);

#ifndef DEBUG
    if (contId > m_DebugControllerId) {
        return true;
    }
#endif
    if (!IsControllerConnected(contId)) {
        return true;
    }
    for (int i = 0; i < NGPS_MAX_BUTTONS; i++) {
        if (IsPressed(contId, i)) {
            return false;
        }
    }
    return true;
}

void InputSystem::GetJoystickVector(int contId, int joyId, float *x, float *y, int mode) {
    float low, high;

    DEBUGASSERTLINE(254, contId >= 0 && contId < PAD_MAX_CONTROLLERS);
    DEBUGASSERTLINE(255, joyId == NGPS_JOY_LEFT || joyId == NGPS_JOY_RIGHT);

#ifndef DEBUG
    if (contId > m_DebugControllerId) {
        *x = 0.0f;
        *y = 0.0f;
        return;
    }
#endif
    if (!IsControllerConnected(contId)) {
        if (x) {
            *x = 0.0f;
        }
        if (y) {
            *y = 0.0f;
        }
        return;
    }
    low  = 0.0f;
    high = 0.0f;
    if (mode != 0) {
        Reset(&low, &high);
        SetDeadzone(0.0f, 0.0f);
    }
    if (joyId == 0) {
        if (x) {
            *x = GetAnalogStickInput(contId, kAnalogStickX);
        }
        if (y) {
            *y = GetAnalogStickInput(contId, kAnalogStickY);
        }
    } else {
        if (x) {
            *x = GetAnalogStickInput(contId, kAnalogSubStickX);
        }
        if (y) {
            *y = GetAnalogStickInput(contId, kAnalogSubStickY);
        }
    }
    if (mode != 0) {
        SetDeadzone(low, high);
    }
}
