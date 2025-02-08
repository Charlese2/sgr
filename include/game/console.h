typedef bool (*Command)(void);

enum command_type {
    TOGGLE,
    SET_INT,
    SET_FLOAT,
    CALL
};

typedef struct console_command {
    char* command;
    char* description;
    Command* function;
    command_type type;
} console_command;

static console_command console_commands[100];

void print_commands_to_file();
void print_commands_to_tty();
void register_command(console_command* console_command, char* command, char* description, command_type type, Command command_function);
void run_script_file(void);
