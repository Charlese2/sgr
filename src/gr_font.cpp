#include "game/gr.h"
#include "game/gr_font.h"
#include "game/FileSystem.h"
#include "game/living_entity.h"
#include "game/macros.h"

gr_font mempool_info_text;

void gr_font::DrawTextOnScreen2D(int width_offset, int height_offset, const char *text, int font_face_index) {
    int left_pos;
    int top_pos;
    char character;

    left_pos = width_offset;
    top_pos  = height_offset;
    if (text) {
        left_pos = width_offset;
        gRenderSystem.SetupTextureDrawIn3DSpace();
#ifdef DEBUG
        gRenderSystem.StartDraw2D();
#else
        gRenderSystem.StartDraw2D(false);
#endif

        for (; text; text++) {
            character = *text;
        }
    }
}

void gr_font::draw_mempool_info_on_new_line(const char *text, int font_face_index) {
    DrawTextOnScreen2D(mempool_info_text.current_width_offset, mempool_info_text.current_height_offset, text, font_face_index);
}
