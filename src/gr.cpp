#include "game/gr.h"
#include "game/effect_mem.h"
#include "game/console.h"
#include <stdio.h>

bool gr_initialized;
bool Gr_show_stats;
bool Gr_show_paging;
int Gr_ngps_hires = 1;

console_command toggle_show_stats_gr("gr_show_stats", "Toggles Gr_show_stats", TOGGLE, toggle_gr_show_stats);
#ifdef DEBUG
console_command toggle_show_paging_gr("gr_show_paging", "Toggles Gr_show_paging", TOGGLE, toggle_gr_show_paging);
#endif
console_command set_ngps_hires_gr("ngps_hires", "Sets Gr_ngps_hires", TOGGLE, set_gr_ngps_hires);

int toggle_gr_show_stats(void) {
    if (calling_a_command_function) {
        process_command(193);
        if ((arg_type & 0x40)) {
            Gr_show_stats = true;
        } else if (arg_type & 0x80) {
            Gr_show_stats = false;
        } else if ((arg_type & 1)) {
            Gr_show_stats ^= true;
        }
    }

    if (doing_help_for_comand) {
        sprintf(string_buffer, "Usage: %s [bool]\nSets %s to true or false. If nothing passed, then toggles it\n", "gr_show_stats",
                "Gr_show_stats");
        print_to_console(string_buffer, false);
    }

    if (checking_status_for_command) {
        sprintf(string_buffer, "%s is %s\n", "gr_show_stats", Gr_show_stats ? "TRUE" : "FALSE");
        print_to_console(string_buffer, false);
    }

    return Gr_show_stats;
}

int toggle_gr_show_paging(void) {
    if (calling_a_command_function) {
        process_command(193);
        if ((arg_type & 0x40)) {
            Gr_show_paging = true;
        } else if (arg_type & 0x80) {
            Gr_show_paging = false;
        } else if ((arg_type & 1)) {
            Gr_show_paging ^= true;
        }
    }

    if (doing_help_for_comand) {
        sprintf(string_buffer, "Usage: %s [bool]\nSets %s to true or false. If nothing passed, then toggles it\n", "gr_show_paging",
                "Gr_show_paging");
        print_to_console(string_buffer, false);
    }

    if (checking_status_for_command) {
        sprintf(string_buffer, "%s is %s\n", "gr_show_paging", Gr_show_paging ? "TRUE" : "FALSE");
        print_to_console(string_buffer, false);
    }

    return Gr_show_paging;
}

int set_gr_ngps_hires(void) {
    if (calling_a_command_function) {
        process_command(8);
        Gr_ngps_hires = command_argument_int;
    }

    if (doing_help_for_comand) {
        sprintf(string_buffer, "Usage: %s [int]\nSets %s to the specified value.\n", "ngps_hires", "Gr_ngps_hires");
        print_to_console(string_buffer, false);
    }

    if (checking_status_for_command) {
        sprintf(string_buffer, "%s is %d\n", "ngps_hires", Gr_ngps_hires);
        print_to_console(string_buffer, false);
    }
    return Gr_ngps_hires;
}

bool getGrInitialized(void) { return gr_initialized; }
