#include "game/CrankyInput.h"
#include "global.h"
#include "dolphin/types.h"

class InputSystem : CrankyInput {
    public:
    virtual ~InputSystem();
    int unk[80];
    InputSystem();
};

STATIC_ASSERT(sizeof(InputSystem) == 0x154);
