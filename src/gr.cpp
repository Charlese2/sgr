#include "game/gr.h"
#include "game/gr_ngc.h"
#include "game/effect_mem.h"
#include "game/console.h"
#include "game/frametime.h"
#include "game/macros.h"
#include <stdio.h>
#include <string.h>

bool gr_initialized;
bool Gr_show_stats;
bool Gr_show_paging;

int Gr_ngps_hires = 1;

gr gGr;

console_command toggle_show_stats_gr("gr_show_stats", "Toggles Gr_show_stats", TOGGLE, gr::toggle_gr_show_stats);
#ifdef DEBUG
console_command toggle_show_paging_gr("gr_show_paging", "Toggles Gr_show_paging", TOGGLE, gr::toggle_gr_show_paging);
#endif
console_command set_ngps_hires_gr("ngps_hires", "Sets Gr_ngps_hires", SET_INT, gr::set_gr_ngps_hires);

int gr::toggle_gr_show_stats(void) {
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

int gr::toggle_gr_show_paging(void) {
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

bool gr::init(int width, int height, int unk3, int window_mode, bool unk5, bool unk6) {
    memset(&gGr, 0, sizeof(gr));
    gGr.unkC          = unk3;
    gGr.m_window_mode = window_mode;
    gGr.m_width       = width;
    gGr.m_height      = height;
    gGr.unk18         = 1.0f;
    gGr.unk28         = 0;
    gGr.unk2C         = 0;
    gGr.m_left        = 0;
    gGr.m_top         = 0;
    gGr.unk44         = gGr.m_width - 1;
    gGr.unk4C         = gGr.m_height - 1;
    gGr.unk30         = gGr.m_width;
    gGr.unk34         = gGr.m_height;
    gGr.unk54         = -1;
    gGr.unk58         = -1;
    gGr.unk6C         = unk6;
    gGr.unk70         = 0;

    switch (gGr.m_window_mode) {
    case 200:
    case 201:
    case 202:
        break;
    default:
        DEBUGERRORLINE(152, "Invalid window_mode passed to gr_init\n");
    }

    gr_initialized = true;

    DrawLoadingScreenBackground();
    if (unk5) {
        DrawScreen();
    }
    return false;
}

int gr::set_gr_ngps_hires(void) {
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

Color::Color(s32 red, s32 green, s32 blue, s32 alpha) {
    this->red   = red;
    this->green = green;
    this->blue  = blue;
    this->alpha = alpha;
}

void gr::SetBounds(int left, int top, int width, int height) {
    gGr.m_left = 0;
    gGr.unk44  = width - 1;
    gGr.m_top  = 0;
    gGr.unk4C  = height - 1;
    if (left < 0) {
        gGr.m_left = -left;
    } else if (gGr.m_width - 1 < left) {
        gGr.m_left = gGr.m_width - 1 - left;
    }
    if (gGr.unk44 + left < 0) {
        gGr.unk44 = -left;
    } else if (gGr.m_width - 1 <= gGr.unk44 + left) {
        gGr.unk44 = gGr.m_width - 1 - left;
    }
    if (top < 0) {
        gGr.m_top = -top;
    } else if (gGr.m_height -1 < top) {
        gGr.m_top = gGr.m_height - 1 - top;
    }
    if (gGr.m_top + top < 0) {
        gGr.m_top = -top;
    } else if (gGr.m_width - 1 <= gGr.m_top + top) {
        gGr.m_top = gGr.m_width - 1 - top;
    }
    gGr.unk30 = gGr.unk44 - gGr.m_left + 1;
    gGr.unk34 = gGr.unk4C - gGr.m_top + 1;
    gGr.unk28 = left;
    gGr.unk2C = top;
    gr_ngc::CalculateScissor();
}

void gr::CalculateScreenBounds() { SetBounds(0, 0, gGr.m_width, gGr.m_height); }

void gr::DrawLoadingScreenBackground() { DrawBlackBackground(); }

void gr::SetDynamicTextureColor(u32 red, u32 green, u32 blue, u32 alpha) {
    gGr.m_dynamic_texture_color.red   = red;
    gGr.m_dynamic_texture_color.green = green;
    gGr.m_dynamic_texture_color.blue  = blue;
    gGr.m_dynamic_texture_color.alpha = alpha;
}

void gr::DrawHudColor(Color *color) { SetDynamicTextureColor(color->red, color->green, color->blue, color->alpha); }

void gr::rect(s32 start_x_position, s32 start_y_position, s32 x_size, s32 y_size) {
    gr_ngc::DrawDynamicTexture(start_x_position, start_y_position, start_x_position + x_size, start_y_position + y_size);
}

bool gr::getInitialized(void) { return gr_initialized; }

void gr::DrawBlackBackground() {
    if (!gRenderSystem.GetCurrentMode(kRenderModeDraw)) {
        gRenderSystem.StartDraw();
    }
    if (!gRenderSystem.GetCurrentMode(kRenderModeDraw2D)) {
        gRenderSystem.StartDraw_2D(false);
    }
    SetDynamicTextureColor(0, 0, 0, 255);
    rect(0, 0, 512, 448);
}
