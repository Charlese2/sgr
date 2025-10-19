#include "game/RenderSystem.h"

extern float frametime;

class gr {};

void GraphicsInit(int, int, int, int window_mode, int, int);
int toggle_gr_show_stats(void);
int toggle_gr_show_paging(void);
int set_gr_ngps_hires(void);
bool getGrInitialized(void);
void DrawTextOnScreen2D(int left_offset, int top_offset, char *text, int param_4);
void draw_mempool_info_on_new_line(char *text, int param_2);
