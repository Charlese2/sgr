#include "game/console.h"
#include "game/gr.h"
#include "game/macros.h"

char next_arg[256];
char original_arg[256];
console_command* Console_commands[100];
s32 calling_a_command_function;
s32 doing_help_for_comand;
s32 checking_status_for_command;
s32 current_command_count;
BOOL debug_command_prefixed;
bool console_is_animating;
bool console_is_showing;
int command_argument_int;
float command_argument_float;
GXColor console_background_color;
u32 arg_type;

extern char string_buffer[512];

BOOL console_command::add_command(char *name, char *description, command_type type) {
    if (current_command_count >= MAX_COMMANDS) {
        DEBUGINT3LINE(641);
        return false;
    }

    int i;
    for (i = 0; i < current_command_count; i++) {
        int found = stricmp(Console_commands[i]->name, name);
        if (found == 0)
        {
            DEBUGINT3LINE(649);
            return false;
        }

        if (found > 0) {
            break;
        }

        if (found < 0) {
            continue;
        }
    }

    int j;
    if (i < current_command_count) {
        for (j = current_command_count; j > i; j--) {
            Console_commands[j] = Console_commands[j-1];
        }
        Console_commands[i] = this;
        current_command_count++;
    }
    else {
        Console_commands[current_command_count] = this;
        current_command_count++;
    }



    this->name = name;
    this->description = description;
    this->type = type;
    return true;
}

console_command::console_command(char *command, char *description, command_type type, CommandCallback command_function) {
    if (this->add_command(command, description, type)) {
        callback = command_function;
    }
}

void print_console_commands_to_file(void) {
    if (calling_a_command_function) {

    }

    if (doing_help_for_comand) {
        print_to_console("Print out console commands to the text file console_commands.txt", false);
    }
}

void print_commands_to_file() {

}

void print_commands_to_tty() {

}

void print_to_console(char * buffer, bool unk) {

}

void process_command(int commandId) {
    if (debug_command_prefixed) {
        sprintf(string_buffer, "next arg is '%s', was originally '%s'\n", &next_arg, &original_arg);
        print_to_console(string_buffer, false);
        print_to_console("Rest of the command line is ", false);

        print_to_console("\n", false);
    }
}

void run_script_file() {

}
