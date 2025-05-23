#include "metrotrk/msghndlr.h"

extern void OSReport(const char*);

void usr_puts_serial(const char* msg) {
    char current_char;
    char buffer[2];
    BOOL connection_state;

    while (TRUE) {
        current_char = *msg;
        msg++;
        if (current_char == 0) {
            break;
        }
        connection_state = GetTRKConnected();
        SetTRKConnected(FALSE);
        buffer[1] = 0;
        buffer[0] = current_char;
        OSReport(buffer);
        SetTRKConnected(connection_state);
    }

}

void usr_put_initialize() {}
