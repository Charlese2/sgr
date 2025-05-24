#include "game/CrankyInput.h"
#include "global.h"
#include "dolphin/types.h"

class InputSystem : CrankyInput {
    virtual void unk0();
    int unk[80];
    public:
    InputSystem();
    ~InputSystem();
};

STATIC_ASSERT(sizeof(InputSystem) == 0x154);
