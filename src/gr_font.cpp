#include "game/gr.h"
#include "game/FileSystem.h"
#include "game/living_entity.h"
#include "game/macros.h"

void DrawTextOnScreen2D(int width_offset, int height_offset, char *text, int param_4) {
    int left_pos;
    int top_pos;
    char character;

    left_pos = width_offset;
    top_pos = height_offset;
    if (text) {
        left_pos = width_offset;
        gRenderSystem.SetupTextureDrawIn3DSpace();
        gRenderSystem.Draw();

        for (; text; text++) {
            character = *text;
        }

    }

}

void draw_mempool_info_on_new_line(char *text, int param_2) {

}
