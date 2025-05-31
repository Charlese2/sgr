#include "game/mouse.h"
#include "game/macros.h"
#include "game/InputSystem.h"
#include <stdio.h>

bool mouse_initalized;
mouse mouse_buttons[MOUSE_NUM_BUTTONS];
mouse mouse_buttons2[MOUSE_NUM_BUTTONS];
int screen_height;
int screen_width;
int mouse_position_x;
int mouse_position_y;
int mouse_position_z;
int maybe__mouse_id;
int position_x_delta;
int position_y_delta;
int position_z_delta;
int unused_z;
int previous_mouse_x_position;
char mouse_x_position_initialized;
int previous_mouse_y_position;
char mouse_y_position_initialized;

void MoveMouse() {
    float x;
    float y;
    if (mouse_x_position_initialized == 0) {
        previous_mouse_x_position    = 0;
        mouse_x_position_initialized = true;
    }
    if (!mouse_y_position_initialized) {
        previous_mouse_y_position    = 0;
        mouse_y_position_initialized = true;
    }
    position_x_delta = position_y_delta = position_z_delta = 0;
    if (mouse_initalized) {
        gInputSystem.GetJoystickVector(0, 0, &x, &y, 0);
        y = -y;
        mouse_position_x          = ((x + 1.0f) * 0.5f) * 512.0f;
        mouse_position_y          = ((y + 1.0f) * 0.5f) * 448.0f;
        position_x_delta          = mouse_position_x - previous_mouse_x_position;
        position_y_delta          = mouse_position_y - previous_mouse_y_position;
        previous_mouse_x_position = mouse_position_x;
        previous_mouse_y_position = mouse_position_y;
    }
}

void initialize_mouse(int height, int width) {
    if (!mouse_initalized) {
        mouse_initalized = true;
        screen_height    = height;
        screen_width     = width;
        mouse_position_x = screen_height / 2;
        mouse_position_y = screen_width / 2;
        maybe__mouse_id  = 0;
        MoveMouse();
        Unknown();
    }
}

void Unknown() {
    if (mouse_initalized) {
        MoveMouse();
        position_x_delta = position_y_delta = position_z_delta = 0;
        for (int i = 0; i < 3; i++) {
            mouse_buttons[i].unk4 = mouse_buttons[i].unk8 = mouse_buttons[i].unk1 = 0;
            mouse_buttons2[i].timer.SetTimeout(0);
            mouse_buttons2[i].unk0 = false;
        }
    }
}

int Unknown2(int *x_position, int *y_position, int *z_position) {
    if (!mouse_initalized) {
        *x_position = *y_position = *z_position = 0;
        return -1;
    }
    *x_position = mouse_position_x;
    *y_position = mouse_position_y;
    *z_position = mouse_position_z;
    return 0;
}

bool CheckMouseUnknown1(int button) {
    if (!mouse_initalized) {
        return false;
    }
    DEBUGASSERTLINE(305, (button >= 0) && (button < MOUSE_NUM_BUTTONS));
    return mouse_buttons[button].unk0;
}

bool CheckMouseUnknown2(int button) {
    if (!mouse_initalized) {
        return false;
    }
    DEBUGASSERTLINE(359, (button >= 0) && (button < MOUSE_NUM_BUTTONS));
    return mouse_buttons[button].unk4 > 0;
}

void Unknown3(int *x_position, int *y_position, int *z_position) {
    *x_position = position_x_delta;
    *y_position = position_y_delta;
    *z_position = position_z_delta;
}
