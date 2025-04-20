#include "dolphin/types.h"
#include "dolphin/gx/GXStruct.h"
#include "game/RenderSystem.h"

bool getGrInitialized(void);
int is_current_mode(RenderSystem * render_system, u32 mode);
void DrawTextOnScreen(int left_offset, int top_offset, char *text, int param_4);
void draw_mempool_info_on_new_line(char *text, int param_2);
