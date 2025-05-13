#include "dolphin/types.h"

#define MAX_COMMANDS 300
#define MAX_CONSOLE_LINE_LENGTH 64

typedef int (*CommandCallback)(void);

enum command_type {
    TOGGLE,
    SET_INT,
    SET_FLOAT,
    CALL
};

class console_command {
    public:
    const char* name;
    const char* description;
    CommandCallback callback;
    command_type type;

    console_command(const char *command, const char *description, command_type type, CommandCallback command_function);

    BOOL add_command(const char *name, const char *description,command_type type);
};

extern s32 calling_a_command_function;
extern s32 doing_help_for_comand;
extern s32 checking_status_for_command;
extern int command_argument_int;
extern float command_argument_float;
extern u32 arg_type;

int print_commands_to_file(void);
int print_commands_to_tty(void);

void print_to_console(const char* buffer, bool unk);
void process_command(int commandId);
int run_script_file(void);
void load_script(char * script_file);
