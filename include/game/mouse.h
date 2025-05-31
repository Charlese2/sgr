#include "dolphin/types.h"
#include "game/timestamp.h"

#define MOUSE_NUM_BUTTONS 3

class mouse {
    public:
    bool unk0;
    bool unk1;
    int unk4;
    int unk8;
    RealtimeTimer timer;
};

void MoveMouse();
void initialize_mouse(int height, int width);
void Unknown();
int Unknown2(int *x_position, int *y_position, int *z_position);
bool CheckMouseUnknown1(int button);
bool CheckMouseUnknown2(int button);
void Unknown3(int *x_position, int *y_position, int *z_position);
