#include "game/Bitmap.h"
#include "dolphin.h"
#include "game/text_effects.h"

#define MAX_FONT_FILENAME_LENGTH 32
#define MAX_FONTS                2

class glyph_data {
  public:
    int x;
    int y;
    int unkC;
    u16 unk10;
};

class font {
  public:
    char m_font_filename[MAX_FONT_FILENAME_LENGTH];
    int unk20;
    int unk24;
    int m_numChars;
    int m_stride;
    int unk30;
    int m_fontHeight;
    int unk38;
    int unk3C;
    int unk40;
    int unk44;
    glyph_data *m_pCharData;
    int unk4C;
    Bitmap *m_bmpHandle;
    int *m_pCharX;
    int *m_pCharY;
};

class gr_font {
  public:
    static int GetFontHeight(int font_number);
    static void setup_text_effect(int *height, int *width, const char *text, int remaining_time, int font_face_index);
    static int get_character_glyph_position(u8 character, u8 text, int &width, int &height);
    static void DrawTextOnScreen2D(int width_offset, int height_offset, const char *text, int font_face_index);
    static void DrawTextOnScreen3D(char *text, int font_number, float x, float y, float z);
    static bool register_typeface(const char *typeface, font *font);
    static void draw_mempool_info_on_new_line(const char *text, int font_face_index);
    static int load(const char *typeface);
};
