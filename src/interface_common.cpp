#include "game/interface_common.h"
#include "game/gamewide.h"
#include "game/gr.h"
#include "game/gr_ngc.h"

float black_bar_alpha;

void DrawBlackBars() {
    if (!(Game_mode & 0x100)) {
        return;
    }
    if (black_bar_alpha <= 0.0f || black_bar_alpha >= 1.0f) {
        return;
    }
    gr_ngc::set_alpha_blending_mode(1);
    gr_ngc::set_z_mode(0);
    int left, top, width, height;
    gr::GetBounds(left, top, width, height);
    gr::CalculateScreenBounds();
    gr::SetDynamicTextureColor(0, 0, 0, Math::round_float_to_int(255.0f * black_bar_alpha));
    int end_y = black_bar_content_rect.end_y_position;
    int end_x_position = 512;
    int end_y_position = (448 - end_y) / 2;
    int start_x_position = 0;
    int start_y_position = 0;
    gr::rect(start_x_position, start_y_position, end_x_position, end_y_position);
    start_y_position = 448 - end_y_position;
    gr::rect(start_x_position, start_y_position, end_x_position, end_y_position);
    gr::SetBounds(left, top, width, height);
}
