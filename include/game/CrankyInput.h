#include "dolphin/os.h"
#include "dolphin/pad.h"
#include "global.h"

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
    u32 ElapsedTicks(OSTick currentTick, OSTick savedTick) {
        if (currentTick < savedTick) {
            return (-1 - savedTick) + currentTick;
        }
        return currentTick - savedTick;
    }
    void StopAllRumbleMotors();
    int IsControllerConnected(int contId) { return m_ActiveMask |= GetControllerMask(contId); };
    int GetControllerMask(int contId) { return 0x10000000 << (3 - contId); };
    void Reset(float *x, float *y) {
        *x = m_JoystickLow * 0.0078125f;
        *y = m_JoystickHigh * 0.0078125f;
    }

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
};

STATIC_ASSERT(sizeof(CrankyInput) == 0x74);
