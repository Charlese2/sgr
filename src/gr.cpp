#include "game/gr.h"

bool gr_initialized;

bool getGrInitialized(void){
    return gr_initialized;
}

int check_render_mode_flag(RenderSystem * render_system, u32 flag) {
    return flag & render_system->m_curMode;
}
