#include "game/InputSystem.h"
#include "game/NGCSystem.h"
#include "game/new_math.h"
#include "dolphin/pad.h"
#include "dolphin/os.h"
#include "game/macros.h"
#include <string.h>

extern bool ResetGame;

InputSystem gInputSystem;
bool InputSystem_unknown_global_bool = true;

InputSystem::InputSystem() {
    m_LeftTriggerDown      = false;
    m_RightTriggerDown     = false;
    m_unk1                 = false;
    m_unk2                 = true;
    m_unk3                 = true;
    m_unk4                 = true;
    m_DebugControllerId    = 0;
    m_DebounceTicks        = OSMillisecondsToTicks(50);
    m_CurrentTick          = 0;
#ifdef DEBUG
    m_GameResetSavedTick   = 0;
    m_GameResetCurrentTick = 0;
#else
    m_unk5                 = false;
#endif  
    m_buttons[0]           = 0;
    m_buttons[1]           = 0;
    m_buttons[2]           = 0;
    m_buttons[3]           = PAD_BUTTON_START;
    m_buttons[4]           = PAD_BUTTON_UP;
    m_buttons[5]           = PAD_BUTTON_RIGHT;
    m_buttons[6]           = PAD_BUTTON_DOWN;
    m_buttons[7]           = PAD_BUTTON_LEFT;
    m_buttons[8]           = 0;
    m_buttons[9]           = PAD_TRIGGER_Z;
    m_buttons[10]          = PAD_TRIGGER_L;
    m_buttons[11]          = PAD_TRIGGER_R;
    m_buttons[12]          = PAD_BUTTON_Y;
    m_buttons[13]          = PAD_BUTTON_X;
    m_buttons[14]          = PAD_BUTTON_A;
    m_buttons[15]          = PAD_BUTTON_B;
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
    return CrankyInput::is_button_pressed(contId, m_buttons[ButtonId], true);
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
        GetDeadzone(&low, &high);
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

void InputSystem::RunSystem(bool unk) {
    OSTick tick;
    StickTicks *pInputTicks;
    float x, y;

    if (unk) {
        DriveStatus(0, 0);
    }
    tick = OSGetTick();
    if (m_CurrentTick != 0) {
        if (ElapsedTicks(tick, m_CurrentTick) < OSMicrosecondsToTicks(16600)) {
            return;
        }
    }
    m_CurrentTick = tick;
#ifdef DEBUG
    RunRumbleMotorTimer();
    if (is_button_pressed(CONTROLLER_ONE, PAD_BUTTON_X, false) && is_button_pressed(CONTROLLER_ONE, PAD_BUTTON_B, false) &&
        is_button_pressed(CONTROLLER_ONE, PAD_BUTTON_START, false)) {
        if (m_GameResetSavedTick == 0) {
            m_GameResetSavedTick = OSGetTick();
        }

    } else if (m_GameResetSavedTick != 0) {
        m_GameResetCurrentTick = OSGetTick();
    }
    if (m_GameResetSavedTick != 0 && m_GameResetCurrentTick != 0) {
        if (OSTicksToMilliseconds(ElapsedTicks(m_GameResetCurrentTick, m_GameResetSavedTick)) >= 500) {
            ResetGame = true;
        }
        m_GameResetSavedTick = 0;
        m_GameResetCurrentTick = 0;
    }
#else
    ResetGame = false;
    RunRumbleMotorTimer();
#endif
    for (int contId = 0; contId < PAD_MAX_CONTROLLERS; contId++) {
        if (is_controller_connected(contId)) {
            for (int joyId = 0; joyId < NGPS_JOY_COUNT; joyId++) {
                pInputTicks = &m_InputTicks[contId].Stick[joyId];
                GetJoystickVector(contId, joyId, &x, &y, 0);
                if (y < 0.0f) {
                    if (pInputTicks->Direction[DIRECTION_DOWN] == 0) {
                        pInputTicks->Direction[DIRECTION_DOWN] = m_Tick;
                    }
                    pInputTicks->Direction[DIRECTION_UP] = 0;
                } else if (y > 0.0f) {
                    if (pInputTicks->Direction[DIRECTION_UP] == 0) {
                        pInputTicks->Direction[DIRECTION_UP] = m_Tick;
                    }
                    pInputTicks->Direction[DIRECTION_DOWN] = 0;
                } else {
                    pInputTicks->Direction[DIRECTION_DOWN] = 0;
                    pInputTicks->Direction[DIRECTION_UP]   = 0;
                }
                if (x < 0.0f) {
                    if (pInputTicks->Direction[DIRECTION_LEFT] == 0) {
                        pInputTicks->Direction[DIRECTION_LEFT] = m_Tick;
                    }
                    pInputTicks->Direction[DIRECTION_RIGHT] = 0;
                } else if (x > 0.0f) {
                    if (pInputTicks->Direction[DIRECTION_RIGHT] == 0) {
                        pInputTicks->Direction[DIRECTION_RIGHT] = m_Tick;
                    }
                    pInputTicks->Direction[DIRECTION_LEFT] = 0;
                } else {
                    pInputTicks->Direction[DIRECTION_LEFT]  = 0;
                    pInputTicks->Direction[DIRECTION_RIGHT] = 0;
                }
            }
        }
    }
}

void InputSystem::CopyButtonStatus() {
    u16 buttons[4];
    for (int i = 0; i < PAD_MAX_CONTROLLERS; i++) {
        buttons[i] = m_PadStatus[i].button;
#ifndef DEBUG
        if (i > m_DebugControllerId) {
            continue;
        }
#endif
        InitializeController(i);
    }
    for (int j = 0; j < PAD_MAX_CONTROLLERS; j++) {
        m_ControllerStatus[j].button = buttons[j];
        m_PadStatus[j].button        = buttons[j];
    }
}
