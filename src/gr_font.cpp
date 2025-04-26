#include "game/gr.h"
#include "game/FileSystem.h"
#include "game/living_entity.h"
#include "game/macros.h"

gr_font mempool_info_text;

void DrawTextOnScreen2D(int width_offset, int height_offset, char *text, int param_4) {
    int left_pos;
    int top_pos;
    char character;

    left_pos = width_offset;
    top_pos = height_offset;
    if (text) {
        left_pos = width_offset;
        gRenderSystem.SetupTextureDrawIn3DSpace();
        gRenderSystem.Setup2DElementDraw();

        for (; text; text++) {
            character = *text;
        }

    }

}

void draw_mempool_info_on_new_line(char *text, int param_2) {
    DrawTextOnScreen2D(mempool_info_text.current_width_offset, mempool_info_text.current_height_offset, text, param_2);
}
