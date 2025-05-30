#include "dolphin/types.h"
#include "game/RenderSystem.h"


typedef struct font_face {

} font_face;

class gr {

};

class gr_font {
    public:
    u32 loaded_fonts;
    font_face* font_face;
    s32 current_width_offset;
    s32 current_height_offset;
    int unk14;
};

void GraphicsInit(int,int,int,int window_mode,int,int);
int toggle_gr_show_stats(void);
int toggle_gr_show_paging(void);
int set_gr_ngps_hires(void);
bool getGrInitialized(void);
void DrawTextOnScreen2D(int left_offset, int top_offset, char *text, int param_4);
void draw_mempool_info_on_new_line(char* text, int param_2);
