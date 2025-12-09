#include "game/gr.h"
#include "game/gr_font.h"
#include "game/FileSystem.h"
#include "game/gr_ngc.h"
#include "game/memory.h"
#include "game/living_entity.h"
#include "game/macros.h"
#include <string.h>

int loaded_fonts_num;
font *p_font;
font *p_default_font;
gr_font mempool_info_text;
s32 current_width_offset;
s32 current_height_offset;
font fonts[MAX_FONTS];

void gr_font::DrawTextOnScreen2D(int width_offset, int height_offset, const char *text, int font_face_index) {
    int left_pos;
    int top_pos;
    char character;

    left_pos = width_offset;
    top_pos  = height_offset;
    if (text) {
        left_pos = width_offset;
        gRenderSystem.SetupTextureDrawIn3DSpace();
        gRenderSystem.StartDraw2D();

        for (; text; text++) {
            character = *text;
        }
    }
}

void gr_font::DrawTextOnScreen3D(char *text, int font_number, float x, float y, float size) {
    int text_width;
    int text_height;
    int text_z;
    int char_x;
    int char_y;
    if (font_number < 0) {
        p_font = p_default_font;
    } else {
        p_font = &fonts[font_number];
    }
    if (text == NULL || *text == '\0') {
        return;
    }
    gRenderSystem.SetupTextureDrawIn3DSpace();
    gRenderSystem.StartDraw2D();
    u32 font_height = p_font->m_fontHeight;
    while (*text != '\0') {
        s32 character     = *text++;
        int index         = get_character_glyph_position(character, *text, text_width, text_height);
        glyph_data *pData = &p_font->m_pCharData[index];
        char_x            = p_font->m_pCharX[index];
        char_y            = p_font->m_pCharY[index];
        text_width        = Math::round_float_to_int(text_width * size);
        text_height       = Math::round_float_to_int(text_height * size);
        gr_ngc::DrawStaticTexture(p_font->m_bmpHandle, char_x, char_y, text_width, font_height, Math::round_float_to_int(x),
                                  Math::round_float_to_int(y), text_width, text_height, false);
        x += text_width;
    }
    gRenderSystem.EndDraw2D();
}

bool gr_font::register_typeface(const char *typeface, font *pFont) {
    CrankyFileBuffer file_buffer;
    u32 *pBuf = allocate_in_commonblock(2048);
    DEBUGASSERTLINE(401, pBuf);
    file_buffer.SetFileBuffer(pBuf, 2048);
    file_buffer.Open(GetFilePath(6), typeface);
    DEBUGASSERTLINE(406, strlen(typeface) < MAX_FONT_FILENAME_LENGTH);
    strcpy(pFont->m_font_filename, typeface);
}

int gr_font::load(const char *typeface) {
    int currently_loaded_fonts_num = loaded_fonts_num;
    loaded_fonts_num               = currently_loaded_fonts_num + 1;
    font *pFont                    = &fonts[currently_loaded_fonts_num];
    if (currently_loaded_fonts_num == MAX_FONTS) {
        DEBUGERRORLINE(524, "Too many fonts!\nNeed to change MAX_FONTS.\n");
    }
    if (!register_typeface(typeface, pFont)) {
        loaded_fonts_num--;
        return -1;
    }
    return currently_loaded_fonts_num;
}

void gr_font::draw_mempool_info_on_new_line(const char *text, int font_face_index) {
    DrawTextOnScreen2D(current_width_offset, current_height_offset, text, font_face_index);
}
