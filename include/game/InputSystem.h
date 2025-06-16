#include "dolphin/os.h"
#include "game/CrankyInput.h"
#include "global.h"
#include "dolphin/types.h"
#include "dolphin/pad.h"

#define CONTROLLER_ONE 0
#define CONTROLLER_TWO 1

#define NGPS_JOY_LEFT  0
#define NGPS_JOY_RIGHT 1
#define NGPS_JOY_COUNT 2

#define NGPS_MAX_BUTTONS 16

#define DIRECTION_UP    0
#define DIRECTION_DOWN  1
#define DIRECTION_LEFT  2
#define DIRECTION_RIGHT 3
#define DIR_COUNT       4

class InputSystem : public CrankyInput {
  public:
    virtual ~InputSystem();
    InputSystem();
    u32 IsButtonPressed(int contId, int buttonId);
    void GetJoystickVector(int contId, int joyId, float *x, float *y, int mode);
    bool DebounceTimerExpired(int contId, int joyId, int dir);
    bool IsInDeadzone(int contId, int joyId);
    bool IsNoButtonPressed(int contId);
    void RunSystem(bool unk);
    void CopyButtonStatus();
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
    int m_DebounceTicks;
    int m_DebugControllerId;
    int m_buttons[NGPS_MAX_BUTTONS];
    OSTick m_unkTick1;
#ifdef DEBUG
    OSTick m_unkTick2;
    OSTick m_unkTick3;
#endif
    bool m_unk1;
    bool m_unk2;
    bool m_unk3;
    bool m_unk4;
    bool m_LeftTriggerDown;
    bool m_RightTriggerDown;
    bool m_unk5;
};

extern InputSystem gInputSystem;

STATIC_ASSERT(sizeof(InputSystem) == 328);
