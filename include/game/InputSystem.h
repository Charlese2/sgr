#include "dolphin/os.h"
#include "game/CrankyInput.h"
#include "global.h"
#include "dolphin/types.h"
#include "dolphin/pad.h"

#define NGPS_JOY_LEFT  0
#define NGPS_JOY_RIGHT 1

#define DIR_COUNT 4

typedef struct StickTicks {
    OSTick Direction[4];
} StickTicks;

typedef struct ControllerTicks {
    StickTicks Stick[2];
} InputTicks;

class InputSystem : CrankyInput {
  public:
    virtual ~InputSystem();
    InputSystem();
    void GetJoystickVector(int contId, int joyId, float *x, float *y, int mode);
    bool DebounceTimerExpired(int contId, int joyId, int dir);
    u32 ElapsedTicks(OSTick currentTick, OSTick savedTick);
    void InitializeController(int contId);
    bool Initialize(int debugControllerId) {
      CrankyInput::Initialize(true);
      m_debugControllerId = debugControllerId;
      return true;
    };

  private:
    InputTicks m_InputTicks[PAD_MAX_CONTROLLERS];
    int m_debounceTicks;
    int m_debugControllerId;
    int unk1[16];
    OSTick unkTick1;
    OSTick unkTick2;
    OSTick unkTick3;
    int unk2[3];
};

extern InputSystem gInputSystem;

STATIC_ASSERT(sizeof(InputSystem) == 0x154);
