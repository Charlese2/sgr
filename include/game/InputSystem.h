#include "game/CrankyInput.h"
#include "global.h"
#include "dolphin/types.h"

class InputSystem : CrankyInput {
  public:
    virtual ~InputSystem();
    int unk[80];
    InputSystem();
    void GetJoystickVector(int contId, int joyId, float *x, float *y, int mode);
};

extern InputSystem gInputSystem;

STATIC_ASSERT(sizeof(InputSystem) == 0x154);
