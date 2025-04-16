typedef bool (*Command)(void);

enum command_type {
    TOGGLE,
    SET_INT,
    SET_FLOAT,
    CALL
};

typedef struct {
    char* command;
    char* description;
    Command* function;
    command_type type;
} console_command;

extern bool calling_a_command_function;
extern bool doing_help_for_comand;
extern bool checking_status_for_command;

void print_commands_to_file();
void print_commands_to_tty();
void register_command(console_command* console_command, char* command, char* description, command_type type, Command command_function);
void run_script_file(void);
