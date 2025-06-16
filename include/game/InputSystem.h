#include "dolphin/os.h"
#include "game/CrankyInput.h"
#include "global.h"
#include "dolphin/types.h"
#include "dolphin/pad.h"

#define CONTROLLER_ONE 0
#define CONTROLLER_TWO 1

#define NGPS_JOY_LEFT  0
#define NGPS_JOY_RIGHT 1

#define NGPS_MAX_BUTTONS 16

#define DIR_COUNT 4

typedef struct StickTicks {
    OSTick Direction[4];
} StickTicks;

typedef struct ControllerTicks {
    StickTicks Stick[2];
} InputTicks;

class InputSystem : public CrankyInput {
  public:
    virtual ~InputSystem();
    InputSystem();
    u32 IsButtonPressed(int contId, int buttonId);
    void GetJoystickVector(int contId, int joyId, float *x, float *y, int mode);
    bool DebounceTimerExpired(int contId, int joyId, int dir);
    bool IsInDeadzone(int contId, int joyId);
    bool IsNoButtonPressed(int contId);
    void InitializeController(int contId);
    bool Initialize(int debugControllerId) {
        CrankyInput::Initialize(true);
        m_DebugControllerId = debugControllerId;
        return true;
    };
    BOOL IsPressed(int contId, int buttonId) {
#ifndef DEBUG
        if (contId > m_DebugControllerId) {
            return false;
        }
#endif
        return IsButtonPressed(contId, buttonId);
    };

  private:
    InputTicks m_InputTicks[PAD_MAX_CONTROLLERS];
    int m_DebounceTicks;
    int m_DebugControllerId;
    int buttons[NGPS_MAX_BUTTONS];
    OSTick unkTick1;
    OSTick unkTick2;
#ifdef DEBUG
    OSTick unkTick3;
    int unk2;
#endif
    bool m_LeftTriggerDown;
    bool m_RightTriggerDown;
    int unk3;
};

extern InputSystem gInputSystem;
