#define MAX_COMMANDS 300

typedef bool (*CommandCallback)(void);

enum command_type {
    TOGGLE,
    SET_INT,
    SET_FLOAT,
    CALL
};

typedef struct {
    char* name;
    char* description;
    CommandCallback callback;
    command_type type;
} console_command;

extern bool calling_a_command_function;
extern bool doing_help_for_comand;
extern bool checking_status_for_command;

void print_commands_to_file();
void print_commands_to_tty();
bool add_command(console_command *command,char *name,char *description,command_type type);
console_command* register_command(console_command* console_command, char* command, char* description, command_type type, CommandCallback command_function);
void print_to_console(char* buffer);
void run_script_file(void);
