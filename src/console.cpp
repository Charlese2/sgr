#include "game/console.h"
#include "dolphin/types.h"
#include "game/macros.h"

static console_command* Console_commands[100];
bool calling_a_command_function;
bool doing_help_for_comand;
bool checking_status_for_command;
int current_command_count;

void print_console_commands_to_file(void) {
    if (calling_a_command_function) {

    }

    if (doing_help_for_comand) {
        print_to_console("Print out console commands to the text file console_commands.txt");
    }
}

void print_commands_to_file() {

}

void print_commands_to_tty() {

}

bool add_command(console_command *command,char *name,char *description,command_type type) {
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
        Console_commands[i] = command;
        current_command_count++;
    }
    else {
        Console_commands[current_command_count] = command;
        current_command_count++;
    }



    command->name = name;
    command->description = description;
    command->type = type;
    return true;
}

console_command* register_command(console_command* command, char* name, char* description, command_type type, CommandCallback callback) {
    add_command(command, name, description, type);
    if (command) {
        command->callback = callback;
    }
    return command;
}

void print_to_console(char * buffer) {

}

void run_script_file() {

}
