#include "game/CrankyInput.h"
#include "game/new_math.h"
#include "macros.h"

CrankyInput::CrankyInput() { CrankyInput::Initialize(false); }

CrankyInput::~CrankyInput() {}

void CrankyInput::Initialize(bool static_init) {
    int i;
    int j;
    PADStatus *pPadStatus;

    m_pPadStatus   = NULL;
    m_JoystickLow  = 0.0f;
    m_JoystickHigh = 0.0f;
    m_TriggerLow   = 0.0f;
    m_TriggerHigh  = 0.0f;
    if (static_init) {
        PADInit();
        m_pPadStatus = (PADStatus *)PADRead(m_PadStatus);
        m_Mask       = 0;
        for (i = 0; i < PAD_MAX_CONTROLLERS; i++) {
            pPadStatus = &m_PadStatus[i];
            if (pPadStatus->err == PAD_ERR_NONE || pPadStatus->err == PAD_ERR_TRANSFER) {
                m_ActiveMask |= GetControllerMask(i);
            } else if (pPadStatus->err == PAD_ERR_NO_CONTROLLER) {
                m_Mask |= GetControllerMask(i);
            }
        }
        if (m_Mask != 0) {
            PADReset(m_Mask);
        }
    }
    for (j = 0; j < PAD_MAX_CONTROLLERS; j++) {
        InitializeControllerStatus(j);
    }
}

void CrankyInput::InitializeControllerStatus(int contId) {
    PADStatus *pPadStatus;
    controller_status *pControllerStatus;

    ASSERTLINE(103, contId >= 0 && contId < PAD_MAX_CONTROLLERS);

    pPadStatus                              = &m_PadStatus[contId];
    pPadStatus->button                      = 0;
    pPadStatus->stickX                      = 0;
    pPadStatus->stickY                      = 0;
    pPadStatus->substickX                   = 0;
    pPadStatus->substickY                   = 0;
    pPadStatus->triggerLeft                 = 0;
    pPadStatus->triggerRight                = 0;
    pPadStatus->analogA                     = 0;
    pPadStatus->analogB                     = 0;
    pPadStatus->err                         = 0;
    pControllerStatus                       = &m_ControllerStatus[contId];
    pControllerStatus->rumbleTicksRemaining = 0;
    pControllerStatus->button               = 0;
}

void CrankyInput::RunRumbleMotorTimer() {
    OSTick savedTick;
    OSTick elapsedTicks;
    PADStatus *pPadStatus;
    controller_status *pControllerStatus;
    int i;
    int j;

    for (i = 0; i < PAD_MAX_CONTROLLERS; i++) {
        m_ControllerStatus[i].button = m_PadStatus[i].button;
    }
    m_pPadStatus = (PADStatus *)PADRead(m_PadStatus);
    savedTick    = m_Tick;
    m_Tick       = OSGetTick();
    m_Mask       = 0;
    m_ActiveMask = 0;
    for (j = 0; j < PAD_MAX_CONTROLLERS; j++) {
        pPadStatus        = &m_PadStatus[j];
        pControllerStatus = &m_ControllerStatus[j];
        if (pPadStatus->err == PAD_ERR_NONE || pPadStatus->err == PAD_ERR_TRANSFER) {
            m_ActiveMask |= GetControllerMask(j);
        } else if (pPadStatus->err == PAD_ERR_NO_CONTROLLER) {
            m_Mask |= GetControllerMask(j);
            InitializeControllerStatus(j);
            continue;
        }
        if (pControllerStatus->rumbleTicksRemaining > 0) {
            elapsedTicks = ElapsedTicks(m_Tick, savedTick);
            if (elapsedTicks < pControllerStatus->rumbleTicksRemaining) {
                pControllerStatus->rumbleTicksRemaining -= elapsedTicks;
            } else {
                StopRumbleMotor(j);
            }
        }
    }
    if (m_Mask != 0) {
        PADReset(m_Mask);
    }
}

void CrankyInput::StartRumbleMotor(int contId, u32 rumbleTime) {
    ASSERTLINE(208, contId >= 0 && contId < PAD_MAX_CONTROLLERS);

    if (rumbleTime != 0) {
        m_ControllerStatus[contId].rumbleTicksRemaining = rumbleTime * ((OS_BUS_CLOCK / 4) / 1000);
    } else {
        m_ControllerStatus[contId].rumbleTicksRemaining = 0;
    }
    PADControlMotor(contId, PAD_MOTOR_RUMBLE);
}

void CrankyInput::StopRumbleMotor(int contId) {
    ASSERTLINE(232, contId >= 0 && contId < PAD_MAX_CONTROLLERS);
    PADControlMotor(contId, PAD_MOTOR_STOP);
    m_ControllerStatus[contId].rumbleTicksRemaining = 0;
}

void CrankyInput::StopAllRumbleMotors() {
    for (int i = 0; i < PAD_MAX_CONTROLLERS; i++) {
        StopRumbleMotor(i);
    }
}

float CrankyInput::GetAnalogStickInput(int contId, int analogId) {
    float input;
    PADStatus *pPadStatus;

    ASSERTLINE(268, contId >= 0 && contId < PAD_MAX_CONTROLLERS);
    ASSERTLINE(269, analogId >= kAnalogStickX && analogId <= kAnalogSubStickY);

    pPadStatus = &m_PadStatus[contId];
    input      = 0.0f;
    switch (analogId) {
    case kAnalogStickX:
        input = pPadStatus->stickX;
        break;
    case kAnalogStickY:
        input = pPadStatus->stickY;
        break;
    case kAnalogSubStickX:
        input = pPadStatus->substickX;
        break;
    case kAnalogSubStickY:
        input = pPadStatus->substickY;
        break;
    }
    return ConvertInput(input, m_JoystickLow, m_JoystickHigh, 128.0f);
}

float CrankyInput::ConvertInput(float input, float low, float high, float max) {
    float abs_input = fl_abs(input);
    if (abs_input <= low) {
        return 0.0f;
    } else {
        if (max - abs_input <= high) {
            if (input < 0.0f) {
                input = -(max - high);
            } else {
                input = max - high;
            }
        }
        if (input > 0.0f) {
            input -= low;
        } else if (input < 0.0f) {
            input += low;
        }
    }
    return input / ((max - low) - high);
}

float CrankyInput::GetAnalogTriggerInput(int contId, int analogId) {
    float input;
    PADStatus *pPadStatus;
    ASSERTLINE(307, contId >= 0 && contId < PAD_MAX_CONTROLLERS);
    ASSERTLINE(308, analogId >= kAnalogTriggerLeft && analogId <= kAnalogB);

    pPadStatus = &m_PadStatus[contId];
    input      = 0.0f;
    switch (analogId) {
    case kAnalogTriggerLeft:
        input = pPadStatus->triggerLeft;
        break;
    case kAnalogTriggerRight:
        input = pPadStatus->triggerRight;
        break;
    case kAnalogA:
        input = pPadStatus->analogA;
        break;
    case kAnalogB:
        input = pPadStatus->analogB;
        break;
    }
    return ConvertInput(input, m_TriggerLow, m_TriggerHigh, 255.0f);
}
