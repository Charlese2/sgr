#include "game/gr.h"
#include "game/gr_ngc.h"
#include "game/bitmap.h"
#include "game/FileSystem.h"
#include "game/PackFileSystem.h"
#include "game/debug.h"
#include "dolphin/gx.h"
#include "game/macros.h"

rect black_bar_text_bounds = {0, 74, 512, 298};

void gr_ngc::GetFile(char *filename, int packFileId) {
    if (packFileId < 0) {
        gFileManager.GetFileFromCache(GetFilePath(0), filename);
    } else {
        gPackFileSystem.get_file(filename, packFileId);
    }
}

void gr_ngc::BitmapInfo(Bitmap *bitmapId, int &bmpWidth, int &bmpHeight) {
    u16 width;
    u16 height;
    DEBUGASSERTLINE(122, bitmapId != BM_BOGUS_BITMAP_HANDLE);
    Bitmap *pBitmapId = bitmapId;
    pBitmapId->GetImageHeader().GetImageDimentions(height, width);
    bmpWidth  = width;
    bmpHeight = height;
}

void gr_ngc::DrawLine(float x1, float y1, float z1, float x2, float y2, float z2) {
    GXColor color;
    color.r = gGr.m_dynamic_texture_color.red;
    color.g = gGr.m_dynamic_texture_color.green;
    color.b = gGr.m_dynamic_texture_color.blue;
    color.a = gGr.m_dynamic_texture_color.alpha;
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetNumChans(1);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, color);
    GXBegin(GX_LINES, GX_VTXFMT0, 2);
    GXPosition3f32(x1, y1, z1);
    GXPosition3f32(x2, y2, z2);
    GXEnd();
}

void gr_ngc::DrawLine2D(float x1, float y1, float x2, float y2) {
    gRenderSystem.StartDraw2D();
    GXSetNumChans(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    x1 += gGr.m_left + gGr.m_left_bound;
    y1 += gGr.m_top + gGr.m_top_bound;
    x2 += gGr.m_left + gGr.m_left_bound;
    y2 += gGr.m_top + gGr.m_top_bound;
    y1 = -y1;
    y2 = -y2;
    GXColor color;
    color.r = gGr.m_dynamic_texture_color.red;
    color.g = gGr.m_dynamic_texture_color.green;
    color.b = gGr.m_dynamic_texture_color.blue;
    color.a = gGr.m_dynamic_texture_color.alpha;
    GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, color);
    GXBegin(GX_LINES, GX_VTXFMT0, 2);
    GXPosition3f32(x1, y1, -1.0f);
    GXPosition3f32(x2, y2, -1.0f);
    GXEnd();
    gRenderSystem.EndDraw2D();
}

float gr_ngc::PeekZ(u32 x, u32 y) {
    u32 z;
    GXPeekZ(x, y, &z);
    if (gRenderSystem.IsAAEnabled()) {
        z = GXDecompressZ16(z, GX_ZC_LINEAR);
    }
    float z2 = z / 16777215.0f;
    return z2;
}

void gr_ngc::SetScissor(u32 left, u32 top, u32 width, u32 height) { GXSetScissor(left, top, width, height); }

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
    gRenderSystem.StartDraw2D();
    GXSetNumChans(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    start_x_pixel += gGr.m_left + gGr.m_left_bound;
    start_y_pixel += gGr.m_top + gGr.m_top_bound;
    end_x_pixel += gGr.m_left + gGr.m_left_bound;
    end_y_pixel += gGr.m_top + gGr.m_top_bound;
    float position1 = (float)start_x_pixel;
    float position2 = (float)end_x_pixel;
    float position3 = -(float)start_y_pixel;
    float position4 = -(float)end_y_pixel;
    GXColor color;
    color.r = gGr.m_dynamic_texture_color.red;
    color.g = gGr.m_dynamic_texture_color.green;
    color.b = gGr.m_dynamic_texture_color.blue;
    color.a = gGr.m_dynamic_texture_color.alpha;
    GXSetChanMatColor(GX_COLOR0A0, color);
    GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(position1, position3, -1.0f);
    GXPosition3f32(position2, position3, -1.0f);
    GXPosition3f32(position2, position4, -1.0f);
    GXPosition3f32(position1, position4, -1.0f);
    GXEnd();
    gRenderSystem.EndDraw2D();
}

void gr_ngc::DrawStaticTexture(Bitmap *bmpHandle, const int bmp_section_x, const int bmp_section_y, const int bmp_section_width,
                               const int bmp_section_height, int x, int y, int width, int height, bool unk) {
    if (bmp_section_width == 0 || bmp_section_height == 0) {
        return;
    }
    gRenderSystem.StartDraw2D();
    gRenderSystem.SetupTextureDrawIn3DSpace();
    gRenderSystem.LoadTexture(*bmpHandle, false, GX_TEXMAP0);
    x += gGr.m_left + gGr.m_left_bound;
    y += gGr.m_top + gGr.m_top_bound;
    float position1 = (float)(x);
    float position2 = (float)(x + width);
    float position3 = -(float)y;
    float position4 = -(float)(y + height);
    int bmpWidth    = 0;
    int bmpHeight   = 0;
    BitmapInfo(bmpHandle, bmpWidth, bmpHeight);
    DEBUGASSERTLINE(415, bmpWidth && bmpHeight);
    float coord1 = bmp_section_x / (float)bmpWidth;
    float coord2 = bmp_section_y / (float)bmpHeight;
    float coord3 = (bmp_section_x + bmp_section_width) / (float)bmpWidth;
    float coord4 = (bmp_section_y + bmp_section_height) / (float)bmpHeight;
    GXColor color;
    color.r = gGr.m_dynamic_texture_color.red;
    color.g = gGr.m_dynamic_texture_color.green;
    color.b = gGr.m_dynamic_texture_color.blue;
    color.a = gGr.m_dynamic_texture_color.alpha;
    GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, color);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(position1, position3, -1.0f);
    GXTexCoord2f32(coord1, coord2);
    GXPosition3f32(position2, position3, -1.0f);
    GXTexCoord2f32(coord3, coord2);
    GXPosition3f32(position2, position4, -1.0f);
    GXTexCoord2f32(coord3, coord4);
    GXPosition3f32(position1, position4, -1.0f);
    GXTexCoord2f32(coord1, coord4);
    GXEnd();
    gRenderSystem.EndDraw2D();
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

void gr_ngc::CalculateScissor() {
    u32 left   = gGr.m_left + gGr.m_left_bound;
    u32 width  = gGr.m_drawable_width - gGr.m_left + 1;
    u32 top    = gGr.m_top + gGr.m_top_bound;
    u32 height = gGr.m_drawable_height - gGr.m_top + 1;
    SetScissor(left, top, width, height);
}
