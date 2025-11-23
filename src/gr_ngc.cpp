#include "dolphin/gx/GXEnum.h"
#include "game/gr.h"
#include "game/gr_ngc.h"
#include "game/bitmap.h"
#include "game/debug.h"
#include "dolphin/gx.h"
#include "game/macros.h"

void gr_ngc::set_z_mode(int mode) {
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

void gr_ngc::DrawDynamicTexture(s16 start_x_pixel, s16 start_y_pixel, s16 end_x_pixel, s16 end_y_pixel) {
#ifdef DEBUG
    gRenderSystem.StartDraw2D();
#else
    gRenderSystem.StartDraw2D(false);
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

void gr_ngc::DrawStaticTexture(int *bmpHandle, u32 unk2, u32 unk3, int unk4, int unk5, int unk6, int unk7, int unk8, int unk9, int unk10) {
    if (unk4 && unk5) {
        gRenderSystem.StartDraw_2D(false);
        gRenderSystem.SetupTextureDrawIn3DSpace();
        u32 unknown = gGr.m_left + gGr.m_left_bound + unk6;
        u32 unknown2 = gGr.m_top + gGr.m_top_bound + unk7;
        u32 bmpWidth;
        u32 bmpHeight;
        BitmapInfo(*bmpHandle, bmpWidth, bmpHeight);
        DEBUGASSERTLINE(415, bmpWidth && bmpHeight);
        GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXColor color;
        color.a = gGr.m_dynamic_texture_color.alpha;
        color.b = gGr.m_dynamic_texture_color.blue;
        color.g = gGr.m_dynamic_texture_color.green;
        color.r = gGr.m_dynamic_texture_color.red;
        GXSetChanMatColor(GX_COLOR0A0, color);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(unknown + unk8, unknown2 + unk9, -1.0f);
        GXPosition2f32(unknown + unk2 / bmpWidth, unknown2 + unk3 / bmpHeight);
        GXPosition3f32(unknown, unknown2, -1.0f);
        GXPosition2f32(unknown, unknown2);
        GXPosition3f32(unknown, unknown2, -1.0f);
        GXPosition2f32(unknown, unknown2);
        GXPosition3f32(unknown, unknown2, -1.0f);
        GXPosition2f32(unknown, unknown2);
        GXEnd();
        gRenderSystem.EndDraw2D();
    }
}

void gr_ngc::BitmapInfo(int bitmapId, u32 &bmpWidth, u32 &bmpHeight) {
    u16 width;
    u16 height;
    DEBUGASSERTLINE(122, bitmapId != BM_BOGUS_BITMAP_HANDLE);
    int id = bitmapId;
    Bitmap::GetFromId(id).GetImageDimentions(height, width);
    bmpWidth = width;
    bmpHeight = height;
}

void gr_ngc::set_alpha_blending_mode(int mode) {
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
