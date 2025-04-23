#include "dolphin/types.h"

#define MAX_COMMANDS 300
#define MAX_CONSOLE_LINE_LENGTH 64

typedef bool (*CommandCallback)(void);

enum command_type {
    TOGGLE,
    SET_INT,
    SET_FLOAT,
    CALL
};

class console_command {
    public:
    char* name;
    char* description;
    CommandCallback callback;
    command_type type;

    console_command(char *command,char *description, command_type type, CommandCallback command_function);

    BOOL add_command(char *name,char *description,command_type type);
};

extern s32 calling_a_command_function;
extern s32 doing_help_for_comand;
extern s32 checking_status_for_command;
extern u32 arg_type;

void print_commands_to_file();
void print_commands_to_tty();

void print_to_console(char* buffer, bool unk);
void process_command(int commandId);
void run_script_file(void);
