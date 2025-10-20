#include "game/RenderSystem.h"

class Color {
  public:
    Color(int red, int green, int blue, int alpha);
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
};

class gr {
    public:
    static void GraphicsInit(int, int, int, int window_mode, int, int);
    static int toggle_gr_show_stats(void);
    static int toggle_gr_show_paging(void);
    static int set_gr_ngps_hires(void);
    static void DrawHudColor(Color *color);
    static bool getGrInitialized(void);
    static void DrawTextOnScreen2D(int left_offset, int top_offset, char *text, int param_4);
    static void draw_mempool_info_on_new_line(char *text, int param_2);
};
