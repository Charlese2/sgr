#include "dolphin/types.h"
#include "game/timestamp.h"

#define MOUSE_NUM_BUTTONS 3

class mouse {
    public:
    bool unk0;
    int unk4;
    int unk8;
    RealtimeTimer timer;
};

bool CheckMouseUnknown1(int button);
bool CheckMouseUnknown2(int button);
