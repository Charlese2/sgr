#include "game/gr.h"

bool gr_initialized;

bool getGrInitialized(void){
    return gr_initialized;
}

u32 RenderSystem::GetCurrentMode(u32 flag) {
    return flag & m_curMode;
}
