#include "game/pathfinding_debug.h"
#include "game/effect_mem.h"
#include "game/living_entity.h"
#include "game/console.h"
#include <stdio.h>

vector3 unk1;
vector3 unk2;
pathfinding_debug pathfinding_debug[1000];
vector3 unk4;
vector3 unk5;

float obstacle_height_offset = 0.1;

console_command call_check_ground_mesh("check_ground_mesh", "Check ground mesh against obstacle data", CALL,
                                       (CommandCallbackInt)check_ground_meshs);
console_command call_check_door_splines("check_door_splines", "Check door splines for small/large invert", CALL,
                                        (CommandCallbackInt)check_door_splines);
console_command call_check_number_of_meshes("check_num_meshes", "check number of meshes in query_line_of_sight", CALL,
                                            (CommandCallbackInt)check_number_of_meshes);
console_command call_set_obstacle_height_offset("obstacle_height_offset",
                                                "Set the height level to draw obstacles relative to main player pos", CALL,
                                                (CommandCallbackInt)set_obstacle_height_offset);

void ground_meshes_validation() {}

void door_splines_validation() {}

void check_ground_meshs() {
    if (calling_a_command_function) {
        ground_meshes_validation();
    }
}

void check_door_splines() {
    if (calling_a_command_function) {
        door_splines_validation();
    }
}

void check_number_of_meshes() {
    int x;
    int z;
    if (calling_a_command_function) {
        for (int index = 0; index < 30; index++) {
            float unk = 0.0f;
            int unk2  = 0;
        }
    }
}

void set_obstacle_height_offset() {
    if (calling_a_command_function) {
        process_command(16);
        obstacle_height_offset = command_argument_float;
    }
    if (checking_status_for_command) {
        sprintf(string_buffer, "Obstacle height offset currently at %.3f meters.\n", obstacle_height_offset);
        print_to_console(string_buffer, 0);
    }
    if (doing_help_for_comand) {
        print_to_console("Usage: obstacle_height_offset <distance> -- where distance is in meters.\n", 0);
    }
}
