#include "game/gamewide.h"
#include "game/InputSystem.h"

int PlayVideo(const char *video_file_name, int unk) {
    gInputSystem.is_button_just_pressed(0, PAD_BUTTON_A, true);
    gInputSystem.is_button_just_released(0, PAD_BUTTON_A, true);
}
