#include "game/gr.h"
#include "game/gr_ngc.h"
#include "game/debug.h"
#include "dolphin/gx.h"
#include "game/macros.h"

void set_z_mode(int mode) {
    switch (mode) {
    case 0:
        GXSetZMode(false, GX_ALWAYS, false);
        break;
    case 1:
        GXSetZMode(true, GX_LESS, false);
        break;
    case 2:
        GXSetZMode(true, GX_EQUAL, false);
        break;
    case 3:
        GXSetZMode(false, GX_ALWAYS, true);
        break;
    case 4:
        GXSetZMode(true, GX_LEQUAL, true);
        break;
    default:
        DEBUGASSERTLINE(296, 0);
    }
}

void DrawDynamicTexture(u16 param_1, u16 param_2, u16 param_3, u16 param_4) {
#ifdef DEBUG
    gRenderSystem.Setup2DElementDraw();
#else
    gRenderSystem.Setup2DElementDraw(false);
#endif
    GXSetNumChans(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
}

void set_alpha_blending_mode(int mode) {
    switch (mode) {
    case 1:
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        break;
    case 2:
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_SET);
        break;
    case 0:
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
        break;
    case 3:
        GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR);
        break;
    default:
        DEBUGERRORLINE(746, "Unsupported alpha blending mode");
    }
}
