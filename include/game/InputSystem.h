#include "game/CrankyInput.h"
#include "global.h"
#include "dolphin/types.h"

class InputSystem : CrankyInput {
  public:
    virtual ~InputSystem();
    InputSystem();
    void GetJoystickVector(int contId, int joyId, float *x, float *y, int mode);

  private:
    int unk[80];
};

extern InputSystem gInputSystem;

STATIC_ASSERT(sizeof(InputSystem) == 0x154);
