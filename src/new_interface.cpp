#include "game/interface.h"
#include "game/InputSystem.h"
#include "game/console.h"
#include "game/effect_mem.h"
#include "game/ai.h"
#include "game/inv.h"
#include "game/living_entity.h"
#include "game/camera.h"
#include "game/anim_fx.h"
#include "game/hud.h"
#include <stdio.h>


bool Animator_debug_mode;
bool Debug_frame_dump_protection = true;
int Num_interfaces_showing;

console_command toggle_animator_mode("animator_mode", "Toggles Animator_debug_mode", TOGGLE, new_interface::toggle_debug_animator_mode);
console_command toggle_frame_dump_protection("animator_mode", "Toggles Animator_debug_mode", TOGGLE,
                                             new_interface::toggle_debug_frame_dump_protection);

int new_interface::toggle_debug_animator_mode(void) {
    if (calling_a_command_function) {
        process_command(193);
        if (arg_type & ARGUMENT_TYPE_UNKNOWN7) {
            Animator_debug_mode = true;
        } else if (arg_type & ARGUMENT_TYPE_UNKNOWN8) {
            Animator_debug_mode = false;
        } else if (arg_type & ARGUMENT_TYPE_UNKNOWN1) {
            Animator_debug_mode ^= true;
        }
    }

    if (doing_help_for_comand) {
        sprintf(string_buffer, "Usage: %s [bool]\nSets %s to true or false. If nothing passed, then toggles it\n", "animator_mode",
                "Animator_debug_mode");

        print_to_console(string_buffer, 0);
    }
    if (checking_status_for_command) {
        sprintf(string_buffer, "%s is %s\n", "animator_mode", Animator_debug_mode ? "TRUE" : "FALSE");
        print_to_console(string_buffer, 0);
    }
    return Animator_debug_mode;
}

int new_interface::toggle_debug_frame_dump_protection() {
    if (calling_a_command_function) {
        process_command(193);
        if (arg_type & ARGUMENT_TYPE_UNKNOWN7) {
            Debug_frame_dump_protection = true;
        } else if (arg_type & ARGUMENT_TYPE_UNKNOWN8) {
            Debug_frame_dump_protection = false;
        } else if (arg_type & ARGUMENT_TYPE_UNKNOWN1) {
            Debug_frame_dump_protection ^= true;
        }
    }
    if (doing_help_for_comand) {
        sprintf(string_buffer, "Usage: %s [bool]\nSets %s to true or false. If nothing passed, then toggles it\n", "frame_dump_protection",
                "Debug_frame_dump_protection");
        print_to_console(string_buffer, 0);
    }
    if (checking_status_for_command) {
        sprintf(string_buffer, "%s is %s\n", "frame_dump_protection", Debug_frame_dump_protection ? "TRUE" : "FALSE");
        print_to_console(string_buffer, 0);
    }
    return Debug_frame_dump_protection;
}

void new_interface::DebugButtonsCheck() {
    if (gInputSystem.IsButtonPressed(CONTROLLER_TWO, START_BUTTON)) {
        Ai_paused = !Ai_paused;
    } else if (gInputSystem.IsButtonPressed(CONTROLLER_TWO, Z_TRIGGER_BUTTON)) {
        Players_immortal = !Players_immortal;
    } else if (gInputSystem.IsButtonPressed(CONTROLLER_TWO, R_TRIGGER_BUTTON)) {
        give_party_levels_and_empowered(50);
    } else if (gInputSystem.IsButtonPressed(CONTROLLER_TWO, L_TRIGGER_BUTTON)) {
        Main_entity->give_status(15, 60000, 257, 100.f, -1, false);
    } else if (gInputSystem.IsButtonPressed(CONTROLLER_TWO, X_BUTTON)) {
        Inv_party_gold += 10000;
    }

    if (gInputSystem.IsButtonPressed(CONTROLLER_TWO, DPAD_UP_BUTTON)) {
        if (Camera_slew_mode_enabled) {
            gCamera.SetMode(3);
        } else {
            gCamera.SetMode(0);
        }
        Camera_sway_animation = Camera_slew_mode_enabled;
        Camera_slew_mode_enabled = !Camera_slew_mode_enabled;
        Mouse_cusor_enabled = false;
    }
}

void new_interface::give_party_levels_and_empowered(int levels) {}

void ShowDialogue(char *unk_1, int unk_2, int num_options, int unk_4, DialogueCallback unk_5, int unk_6) {}
