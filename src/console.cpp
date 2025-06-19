#include "game/console.h"
#include "game/effect_mem.h"
#include "game/gr.h"
#include "game/macros.h"

char next_arg[256];
char original_arg[256];
console_command *Console_commands[100];
s32 calling_a_command_function;
s32 doing_help_for_comand;
s32 checking_status_for_command;
u32 arg_type;
int command_argument_int;
float command_argument_float;
s32 current_command_count;
BOOL debug_command_prefixed;
bool console_is_animating;
bool console_is_showing;
GXColor console_background_color;

console_command printCommandsToFile("print_to_file", "Print out console commands to the text file console_commands.txt", CALL,
                                    (CommandCallbackInt)print_commands_to_file);
console_command printCommandsToTTY("print_to_tty", "Print out console commands to the tty window", CALL,
                                   (CommandCallbackInt)print_commands_to_tty);
console_command runScript("script", "Runs a console script file (.vcs)", CALL, (CommandCallbackInt)run_script_file);

void print_commands_to_file(void) {
    char buffer[256];

    if (calling_a_command_function) {
        for (u32 i = 0; i < current_command_count; i++) {
            if (Console_commands[i]) {
                if (Console_commands[i]->description) {
                    sprintf(buffer, "%s - %s", Console_commands[i]->name, Console_commands[i]->description);
                } else {
                    strcpy(buffer, Console_commands[i]->name);
                }
            }
        }
    }

    if (doing_help_for_comand) {
        print_to_console("Print out console commands to the text file console_commands.txt", 0);
    }
}

void print_commands_to_tty(void) {
    s32 prev_len = -1;
    s32 msg_len;
    char buffer[256];

    if (calling_a_command_function) {
        for (u32 i = 0; i < current_command_count; i++) {
            if (Console_commands[i] && (msg_len = strlen(Console_commands[i]->name), prev_len < msg_len)) {
                prev_len = msg_len;
            }
        }
        for (u32 i = 0; i < current_command_count; i++) {
            if (Console_commands[i]) {
                memset(buffer, ' ', 256);
                if (Console_commands[i]->description) {

                    sprintf(buffer, "%s", Console_commands[i]->name);

                    msg_len         = strlen(buffer);
                    buffer[msg_len] = ' ';
                    sprintf(buffer + prev_len + 3, "- %s", Console_commands[i]->description);
                } else {
                    strcpy(buffer, Console_commands[i]->name);
                }
                msg_len = strlen(buffer);
                if (msg_len < 79) {
                    printf("%s\n", buffer);
                } else {
                    msg_len = prev_len + msg_len + 5;
                    DEBUGASSERTLINE(252, msg_len < 255);
                }
                for (; prev_len + 84 <= msg_len; msg_len--) {
                    buffer[msg_len] = buffer[msg_len - (prev_len + 5)];
                }
                for (; msg_len > 78; msg_len--) {
                    buffer[msg_len] = ' ';
                }
                buffer[prev_len + 82] = '|';
                printf("%s\n", buffer);
            }
        }
    }

    if (doing_help_for_comand) {
        print_to_console("Print out console commands to the tty window", 0);
    }
}

BOOL console_command::add_command(const char *name, const char *description, command_type type) {
    if (current_command_count >= MAX_COMMANDS) {
        DEBUGINT3LINE(641);
        return false;
    }

    int i;
    for (i = 0; i < current_command_count; i++) {
        int found = stricmp(Console_commands[i]->name, name);
        if (found == 0) {
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
            Console_commands[j] = Console_commands[j - 1];
        }
        Console_commands[i] = this;
        current_command_count++;
    } else {
        Console_commands[current_command_count] = this;
        current_command_count++;
    }

    this->name        = name;
    this->description = description;
    this->type        = type;
    return true;
}

console_command::console_command(const char *command, const char *description, command_type type, CommandCallbackInt command_function) {
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

void print_available_functions() {
    print_to_console("Available functions:\n", 0);
    for (int i = 0; i < current_command_count; i++) {
        if (Console_commands[i]->description) {
            sprintf(string_buffer, " %s - %s\n", Console_commands[i]->name, Console_commands[i]->description);
        } else {
            sprintf(string_buffer, " %s\n", Console_commands[i]->name);
        }
        print_to_console(string_buffer, 0);
    }
}

void print_to_console(const char *buffer, bool unk) {}

void process_command(int commandId) {
    if (debug_command_prefixed) {
        sprintf(string_buffer, "next arg is '%s', was originally '%s'\n", &next_arg, &original_arg);
        print_to_console(string_buffer, false);
        print_to_console("Rest of the command line is ", false);

        print_to_console("\n", false);
    }
}

void run_script_file(void) {
    if (calling_a_command_function) {
        process_command(2);
        if (strlen(next_arg)) {
            load_script(next_arg);
        } else {
            doing_help_for_comand = 1;
        }
    }

    if (doing_help_for_comand) {
        print_to_console("Usage: script <scriptfilename>\n", 0);
    }
}

void load_script(char *script_file) {}
