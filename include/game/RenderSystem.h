#include "dolphin/gx/GXStruct.h"
#include "dolphin/mtx.h"
#include "dolphin/types.h"

#define kRenderModeDraw 1
#define kRenderModeDraw2D 2

class RenderSystem {
    virtual void unk0(void);
    u32 m_curMode;
    int unk8;
    GXRenderModeObj* m_pRenderMode;
    int unk10;
    int unk14;
    int unk18;
    int unk1C;
    int unk20;
    int unk24;
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    int unk40;
    int unk44;
    int unk48;
    int unk4C;
    int unk50;
    int unk54;
    Mtx m_mtx1;

    public:
    RenderSystem();
    ~RenderSystem();

    u32 GetCurrentMode(u32 flag) {
        return flag & m_curMode;
    }
    void Setup2DElementDraw(void);
    void SetupTextureDrawIn3DSpace(void);
};



extern RenderSystem gRenderSystem;
