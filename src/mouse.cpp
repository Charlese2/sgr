#include "game/mouse.h"
#include "game/macros.h"

bool mouse_initalized;
mouse mouse_buttons[MOUSE_NUM_BUTTONS];
mouse mouse_buttons2[MOUSE_NUM_BUTTONS];

bool CheckMouseUnknown1(int button) {
    if (!mouse_initalized) {
        return NULL;
    }
    DEBUGASSERTLINE(305, (button >= 0) && (button < MOUSE_NUM_BUTTONS));
    return mouse_buttons[button].unk0;
}

bool CheckMouseUnknown2(int button) {
    if (!mouse_initalized) {
        return 0;
    }
    DEBUGASSERTLINE(359, (button >= 0) && (button < MOUSE_NUM_BUTTONS));
    return mouse_buttons[button].unk4 > 0;
}
