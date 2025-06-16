#include "dolphin/os.h"
#include "dolphin/pad.h"
#include "global.h"
#include "macros.h"

#define kAnalogStickX       0
#define kAnalogStickY       1
#define kAnalogSubStickX    2
#define kAnalogSubStickY    3
#define kAnalogTriggerLeft  4
#define kAnalogTriggerRight 5
#define kAnalogA            6
#define kAnalogB            7

typedef struct controller_status {
    OSTick rumbleTicksRemaining;
    u16 button;
} controller_status;

typedef struct StickTicks {
    OSTick Direction[4];
} StickTicks;

typedef struct ControllerTicks {
    StickTicks Stick[2];
} InputTicks;

class CrankyInput {
  public:
    virtual ~CrankyInput();
    CrankyInput();
    void Initialize(bool static_init);
    void InitializeControllerStatus(int contId);
    float GetAnalogStickInput(int contId, int analogId);
    void RunRumbleMotorTimer(void);
    void StartRumbleMotor(int contId, u32 rumbleTime);
    void StopRumbleMotor(int contId);
    float ConvertInput(float input, float low, float high, float max);
    float GetAnalogTriggerInput(int contId, int analogId);
    void SetDeadzone(float low, float high) {
        ASSERTLINE(157, low >= 0.f && low < 1.f);
        ASSERTLINE(158, high >= 0.f && high < 1.f);

        m_JoystickLow  = 128.f * low;
        m_JoystickHigh = 128.f * high;
    };
    u32 ElapsedTicks(OSTick currentTick, OSTick savedTick) {
        if (currentTick < savedTick) {
            return (-1 - savedTick) + currentTick;
        }
        return currentTick - savedTick;
    };
    void StopAllRumbleMotors();
    void GetDeadzone(float *low, float *high) {
        *low  = m_JoystickLow / 128.0f;
        *high = m_JoystickHigh / 128.0f;
    };
    int IsControllerConnected(int contId) { return is_controller_connected(contId); };
    int GetControllerMask(int contId) { return 0x10000000 << (3 - contId); };
    u32 is_controller_connected(int contId) {
        ASSERTLINE(289, contId >= 0 && contId < PAD_MAX_CONTROLLERS);
        
        return m_ActiveMask & GetControllerMask(contId);
    };
    u32 is_button_pressed(int contId, u32 button, bool unk) {
        u8 status;

        ASSERTLINE(315, contId >= 0 && contId < PAD_MAX_CONTROLLERS);

        if (unk) {
            status = 0;
            if ((m_PadStatus[contId].button & button) != 0 && (m_ControllerStatus[contId].button & button) == 0) {
                status = 1;
            }
            return status;
        }
        return m_PadStatus[contId].button & button;
    };
    void InitializeController(int contId) {
        InitializeControllerStatus(contId);
        memset(&m_InputTicks[contId], 0, 32);
    };

    float m_TriggerLow;
    float m_TriggerHigh;
    float m_JoystickLow;
    float m_JoystickHigh;
    PADStatus *m_pPadStatus;
    int m_ActiveMask;
    u32 m_Mask;
    OSTick m_Tick;
    PADStatus m_PadStatus[PAD_MAX_CONTROLLERS];
    controller_status m_ControllerStatus[PAD_MAX_CONTROLLERS];
    InputTicks m_InputTicks[PAD_MAX_CONTROLLERS];
};
