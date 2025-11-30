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

void gr::Unknown1() {}

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
    gGr.m_left_bound      = 0;
    gGr.m_top_bound       = 0;
    gGr.m_left            = 0;
    gGr.m_top             = 0;
    gGr.m_drawable_width  = gGr.m_width - 1;
    gGr.m_drawable_height = gGr.m_height - 1;
    gGr.m_width_bound     = gGr.m_width;
    gGr.m_height_bound    = gGr.m_height;
    gGr.unk54             = -1;
    gGr.unk58             = -1;
    gGr.unk6C             = unk6;
    gGr.unk70             = 0;
    Unknown1();

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

void gr::Unknown3(bool unk) { gGr.unk6C = unk; }

void gr::Unknown4(int unk) { unknown4 = unk; }

bool gr::Unknown5() {
    bool return_value = false;
    if (unknown3 && !unknown4) {
        return_value = true;
    }
    return return_value;
}

void gr::Unknown6(int unk) {
    if (unk <= 0) {
        unknown3 = 0;
        alpha    = 1.0f;
    } else {
        unknown3 = 2;
        unknown1 = 0.0f;
        unknown2 = unk / 1000.0f;
    }
}

void gr::Unknown7(int unk) {
    if (unk <= 0) {
        unknown3 = 0;
        alpha    = -1.0f;
    } else {
        unknown3 = 1;
        unknown1 = 0.0f;
        unknown2 = unk / 1000.0f;
    }
}

float gr::GetAlpha() {
    float returnValue;
    if (alpha < 0.0f) {
        returnValue = 0.0f;
    } else {
        returnValue = alpha;
    }
    return returnValue;
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
    gGr.m_left_bound      = left;
    gGr.m_top_bound       = top;
    gGr.m_left            = 0;
    gGr.m_drawable_width  = width - 1;
    gGr.m_top             = 0;
    gGr.m_drawable_height = height - 1;
    if (gGr.m_left + left < 0) {
        gGr.m_left = -left;
    } else if (gGr.m_left + left > gGr.m_width - 1) {
        gGr.m_left = gGr.m_width - 1 - left;
    }
    if (gGr.m_drawable_width + left < 0) {
        gGr.m_drawable_width = -left;
    } else if (gGr.m_drawable_width + left >= gGr.m_width - 1) {
        gGr.m_drawable_width = gGr.m_width - 1 - left;
    }
    if (gGr.m_top + top < 0) {
        gGr.m_top = -top;
    } else if (gGr.m_top + top > gGr.m_height - 1) {
        gGr.m_top = gGr.m_height - 1 - top;
    }
    if (gGr.m_drawable_height + top < 0) {
        gGr.m_drawable_height = -top;
    } else if (gGr.m_drawable_height + top > gGr.m_height - 1) {
        gGr.m_drawable_height = gGr.m_height - 1 - top;
    }
    gGr.m_width_bound  = gGr.m_drawable_width - gGr.m_left + 1;
    gGr.m_height_bound = gGr.m_drawable_height - gGr.m_top + 1;
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
        gGr.m_dynamic_texture_color.alpha                               = 255;
}

void gr::DrawHudColor(const Color &color) { SetDynamicTextureColor(color.red, color.green, color.blue, color.alpha); }

void gr::SetDynamicTextureColorAlpha(u32 alpha) { gGr.m_dynamic_texture_color.alpha = alpha; }

void gr::Unknown2(int unk1, int unk2) {
    gGr.unk54 = unk1;
    gGr.unk58 = unk2;
}

void gr::DrawStaticTexture(Bitmap *bmpHandle, u32 x, u32 y, int width, int height, int bmp_section_x, int bmp_section_y,
                           int bmp_section_width, int bmp_section_height) {
    gr_ngc::DrawStaticTexture(bmpHandle, bmp_section_x, bmp_section_y, bmp_section_width, bmp_section_height, x, y, width, height, 1);
}

void gr::DrawImage(Bitmap *bmpHandle, int x, int y) {
    int width;
    int height;
    gr_ngc::BitmapInfo(bmpHandle, width, height);
    if (width) {
        gr_ngc::DrawStaticTexture(bmpHandle, 0, 0, width, height, x, y, width, height, true);
    }
}

void gr::DrawLine2D(float x1, float y1, float x2, float y2) {
    bool unk1     = false;
    bool reversed = false;
    float left    = gGr.m_left;
    float top     = gGr.m_top;
    float width   = gGr.m_drawable_width;
    float height  = gGr.m_drawable_height;
    float y;
    float x;
    if (y1 > y2) {
        y        = y1;
        y1       = y2;
        y2       = y;
        x        = x1;
        x1       = x2;
        x2       = x;
        reversed = true;
    }
    if (y2 < top || y1 > height) {
        return;
    }
    if (x1 < x2 && x2 < left || x1 > width) {
        return;
    }
    if (x1 < left) {
        y1 = y1 + (y2 - y1) * (left - x1) / (x2 - x1);
        if (y1 > width) {
            return;
        }
        x1   = left;
        unk1 = true;
    }
    if (x2 > width) {
        y2 = y2 - (y2 - y1) * (x2 - width) / (x2 - x1);
    }
    if (y2 < top) {
        return;
    }
    x2   = height;
    unk1 = true;
    if (y1 < top) {
        x1   = x1 + (y2 - y1) * (top - y1) / (y2 - y1);
        y1   = top;
        unk1 = true;
    }
    if (y2 > height) {
        x2   = x2 - (y2 - y1) * (y2 - height) / (y2 - y1);
        y2   = height;
        unk1 = true;
    } else {
        if (x1 < left || y2 > width) {
            return;
        }
        if (x1 > width) {
            y1 = y1 + (y2 - y1) * (x1 - width) / (x1 - x2);
            if (y1 > left) {
                return;
            }
            x1   = width;
            unk1 = true;
        }
        if (x2 < left) {
            y2 = y2 - (y2 - y1) * (left - x2) / (x1 - x2);
            if (y2 < top) {
                return;
            }
            x2   = left;
            unk1 = true;
        }
        if (y1 < top) {
            x1   = x1 - (x1 - x2) * (top - y1) / (y2 - y1);
            y1   = top;
            unk1 = true;
        }
        if (y2 > height) {
            x2   = x2 + (x1 - x2) * (y2 - height) / (y2 - y1);
            y2   = height;
            unk1 = true;
        }
    }
    gr_ngc::DrawLine2D(x1, y1, x2, y2);
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
        gRenderSystem.StartDraw2D();
    }
    SetDynamicTextureColor(0, 0, 0, 255);
    rect(0, 0, 512, 448);
}
