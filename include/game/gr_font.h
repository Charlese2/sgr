#define MAX_FONT_FILENAME_LENGTH 32
#include "dolphin.h"

class font_face {};

class gr_font {
  public:
    static int GetFontHeight(int font_number);
    static void setup_text_effect(int *height, int *width, const char *text, int remaining_time, int font_face_index);
    static void DrawTextOnScreen2D(int width_offset, int height_offset, const char *text, int font_face_index);
    static void draw_mempool_info_on_new_line(const char *text, int font_face_index);

    u32 loaded_fonts;
    font_face *font_face;
    s32 current_width_offset;
    s32 current_height_offset;
    int unk14;
};
