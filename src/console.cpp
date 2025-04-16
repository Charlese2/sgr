#include "game/console.h"

static console_command console_commands[100];
bool calling_a_command_function;
bool doing_help_for_comand;
bool checking_status_for_command;

void print_commands_to_file() {

}

void print_commands_to_tty() {

}

void register_command(console_command* command_info, char* command, char* description, command_type type, Command command_function) {

}

void run_script_file() {

}
