#include "dolphin/types.h"

class RenderSystem {
    public:
    int unk0;
    u32 m_curMode;

    public:
    RenderSystem();
    ~RenderSystem();
};

extern RenderSystem gRenderSystem;
