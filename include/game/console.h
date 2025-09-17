#include "dolphin/types.h"

#define MAX_COMMANDS            300
#define MAX_CONSOLE_LINE_LENGTH 128

#define ARGUMENT_TYPE_UNKNOWN1 1 << 0
#define ARGUMENT_TYPE_UNKNOWN2 1 << 1
#define ARGUMENT_TYPE_UNKNOWN3 1 << 2
#define ARGUMENT_TYPE_UNKNOWN4 1 << 3
#define ARGUMENT_TYPE_UNKNOWN5 1 << 4
#define ARGUMENT_TYPE_UNKNOWN6 1 << 5
#define ARGUMENT_TYPE_UNKNOWN7 1 << 6
#define ARGUMENT_TYPE_UNKNOWN8 1 << 7

typedef int (*CommandCallbackInt)(void);
typedef void (*CommandCallbackVoid)(void);

enum command_type { TOGGLE, SET_INT, SET_FLOAT, CALL };

class console_command {
  public:
    console_command(const char *command, const char *description, command_type type, CommandCallbackInt command_function);
    BOOL add_command(const char *name, const char *description, command_type type);

    const char *name;
    const char *description;
    CommandCallbackInt callback;
    command_type type;
};

extern char next_arg[256];
extern s32 calling_a_command_function;
extern s32 doing_help_for_comand;
extern s32 checking_status_for_command;
extern int command_argument_int;
extern float command_argument_float;
extern u32 arg_type;

void print_commands_to_file(void);
void print_commands_to_tty(void);

void print_available_functions(void);
void print_to_console(const char *buffer, bool unk);
void process_command(int commandId);
void run_script_file(void);
void load_script(char *script_file);
