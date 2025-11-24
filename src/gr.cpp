#include "game/gr.h"
#include "game/gr_poly.h"
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

float fade_speed = 1.8f;
float alpha      = -1;
float unknown1;
float unknown2;
int unknown3;
int unknown4;

gr gGr;

console_command toggle_show_stats_gr("gr_show_stats", "Toggles Gr_show_stats", TOGGLE, gr::toggle_gr_show_stats);
#ifdef DEBUG
console_command toggle_show_paging_gr("gr_show_paging", "Toggles Gr_show_paging", TOGGLE, gr::toggle_gr_show_paging);
#endif
console_command set_ngps_hires_gr("ngps_hires", "Sets Gr_ngps_hires", SET_INT, gr::set_gr_ngps_hires);

int intTable[256];
float floatTable[256];

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

bool gr::init(int width, int height, int unk3, int window_mode, u32 unk5, bool unk6) {
    memset(&gGr, 0, sizeof(gr));
    gGr.unkC          = unk3;
    gGr.m_window_mode = window_mode;
    gGr.m_width       = width;
    gGr.m_height      = height;
#ifdef DEBUG
    gGr.unk18 = 1.0f;
#else
    gGr.unk18 = 0.85714287f;
#endif
    gGr.m_left_bound   = 0;
    gGr.m_top_bound    = 0;
    gGr.m_left         = 0;
    gGr.m_top          = 0;
    gGr.unk44          = gGr.m_width - 1;
    gGr.unk4C          = gGr.m_height - 1;
    gGr.m_width_bound  = gGr.m_width;
    gGr.m_height_bound = gGr.m_height;
    gGr.unk54          = -1;
    gGr.unk58          = -1;
    gGr.unk6C          = unk6;
    gGr.unk70          = 0;

    switch (gGr.m_window_mode) {
    case 200:
    case 201:
    case 202:
        break;
    default:
#ifdef DEBUG
        DEBUGERRORLINE(152, "Invalid window_mode passed to gr_init\n");
#else
        DEBUGERRORLINE(153, "Invalid window_mode passed to gr_init\n");
#endif
    }
    CreateTables(1.8f);
    gr_initialized = true;
    CalculateScreenBounds();
#ifdef DEBUG
    DrawLoadingScreenBackground();
#endif
    if (unk5) {
        DrawScreen();
    }
    return false;
}

void gr::DrawScreenFade() {
    if (unknown4 == 0) {
        if (unknown3 != 0) {
            unknown1 += reported_frametime;
            if (unknown1 <= unknown2) {
                alpha = unknown1 / unknown2;
                if (unknown3 == 1) {
                    alpha = 1.0f - alpha;
                }
            } else {
                if (unknown3 == 1) {
                    alpha = -1.0f;
                } else {
                    alpha = 1.0f;
                }
                unknown3 = 0;
            }
        }
        if (alpha > 0.0f) {
            gRenderSystem.StartDraw();
            SetDynamicTextureColor(0, 0, 0, 255.0f * alpha);
            gr_ngc::set_alpha_blending_mode(1);
            gr_ngc::set_z_mode(0);
            gr_ngc::SetScissor(0, 0, 516, 452);
            gr_ngc::DrawDynamicTexture(-2, -2, 514, 450);
        }
    }
}

Color::Color(s32 red, s32 green, s32 blue, s32 alpha) {
    this->red   = red;
    this->green = green;
    this->blue  = blue;
    this->alpha = alpha;
}

void gr::SetBounds(int left, int top, int width, int height) {
    gGr.m_left_bound = left;
    gGr.m_top_bound  = top;
    gGr.m_left       = 0;
    gGr.unk44        = width - 1;
    gGr.m_top        = 0;
    gGr.unk4C        = height - 1;
    if (gGr.m_left + left < 0) {
        gGr.m_left = -left;
    } else if (gGr.m_left + left > gGr.m_width - 1) {
        gGr.m_left = gGr.m_width - 1 - left;
    }
    if (gGr.unk44 + left < 0) {
        gGr.unk44 = -left;
    } else if (gGr.unk44 + left >= gGr.m_width - 1) {
        gGr.unk44 = gGr.m_width - 1 - left;
    }
    if (gGr.m_top + top < 0) {
        gGr.m_top = -top;
    } else if (gGr.m_top + top > gGr.m_height - 1) {
        gGr.m_top = gGr.m_height - 1 - top;
    }
    if (gGr.unk4C + top < 0) {
        gGr.unk4C = -top;
    } else if (gGr.unk4C + top > gGr.m_height - 1) {
        gGr.unk4C = gGr.m_height - 1 - top;
    }
    gGr.m_width_bound  = gGr.unk44 - gGr.m_left + 1;
    gGr.m_height_bound = gGr.unk4C - gGr.m_top + 1;
    gr_ngc::CalculateScissor();
}

void gr::GetBounds(int &left, int &top, int &width, int &height) {
    left   = gGr.m_left_bound;
    top    = gGr.m_top_bound;
    width  = gGr.m_width_bound;
    height = gGr.m_height_bound;
}

void gr::CalculateScreenBounds() { SetBounds(0, 0, gGr.m_width, gGr.m_height); }

void gr::DrawLoadingScreenBackground() { DrawBlackBackground(); }

void gr::DrawScreen() {
    gr_poly::unknown1();
    CalculateScreenBounds();
    DrawScreenFade();
    gRenderSystem.EndDraw();
}

void gr::CreateTables(float speed) {
    int i;
#ifdef DEBUG
    fade_speed = speed;
#endif
    for (i = 0; i < 256; i++) {
        int result = Math::powf(i / 255.0f, 1 / fade_speed) * 255;
        if (result > 255) {
            result = 255;
        } else if (result < 0) {
            result = 0;
        }
        intTable[i] = result;
    }
    for (i = 0; i < 256; i++) {
        float result = i;
        if (result > 255) {
            result = 255;
        } else if (result < 0) {
            result = 0;
        }
        floatTable[i] = result;
    }
}

void gr::SetDynamicTextureColor(s32 red, s32 green, s32 blue, s32 alpha) {
    gGr.m_dynamic_texture_color.red   = red;
    gGr.m_dynamic_texture_color.green = green;
    gGr.m_dynamic_texture_color.blue  = blue;
    gGr.m_dynamic_texture_color.alpha = alpha;
}

void gr::ResetDynamicTextureColor() {
    gGr.m_dynamic_texture_color.red = gGr.m_dynamic_texture_color.green = gGr.m_dynamic_texture_color.blue =
        gGr.m_dynamic_texture_color.alpha                                = 255;
}

void gr::DrawHudColor(const Color& color) { SetDynamicTextureColor(color.red, color.green, color.blue, color.alpha); }

void gr::SetDynamicTextureColorAlpha(u32 alpha) {
    gGr.m_dynamic_texture_color.alpha = alpha;
}

void gr::Unknown1(int unk1, int unk2) {
    gGr.unk54 = unk1;
    gGr.unk58 = unk2;
}

void gr::DrawStaticTexture(int bmpHandle, u32 unk2, u32 unk3, int unk4, int unk5, int unk6, int unk7, int unk8, int unk9) {
    gr_ngc::DrawStaticTexture(bmpHandle, unk6, unk7, unk8, unk9, unk2, unk3, unk4, unk5, 1);
}

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
