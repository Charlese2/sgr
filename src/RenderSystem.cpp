#include "game/RenderSystem.h"
#include "dolphin/gx/GXGeometry.h"
#include "dolphin/gx/GXTransform.h"
#include "dolphin/gx/GXLighting.h"
#include "dolphin/gx/GXPixel.h"
#include "dolphin/gx/GXTev.h"
#include "dolphin/mtx.h"
#include "dolphin/os.h"
#include "game/macros.h"

RenderSystem gRenderSystem;

void RenderSystem::UpdateShadowCameraMatrix(Mtx44 *matrix, float field_of_view, float aspect_ratio, float near_clip, float far_clip) {
    float cotangent;
    float size;
    float angle;

    ASSERTMSGLINE(41, matrix, "no matrix present");
    ASSERTMSGLINE(42, field_of_view > 0.0 && field_of_view < 180.0, "invalid field-of-view");
    ASSERTMSGLINE(43, aspect_ratio != 0.0f, "invalid aspect ratio");
    angle           = field_of_view * DEG_TO_RAD(1);
    angle           = 0.5f * angle;
    cotangent       = 1.0f / Math::tanf(angle);
    (*matrix)[0][0] = -(cotangent / aspect_ratio);
    (*matrix)[0][1] = 0.0f;
    (*matrix)[0][2] = 0.0f;
    (*matrix)[0][3] = 0.0f;
    (*matrix)[1][0] = 0.0f;
    (*matrix)[1][1] = cotangent;
    (*matrix)[1][2] = 0.0f;
    (*matrix)[1][3] = 0.0f;
    (*matrix)[2][0] = 0.0f;
    (*matrix)[2][1] = 0.0f;
    size            = 1.0f / (far_clip - near_clip);
    (*matrix)[2][2] = -near_clip * size;
    (*matrix)[2][3] = -(far_clip * near_clip) * size;
    (*matrix)[3][0] = 0.0f;
    (*matrix)[3][1] = 0.0f;
    (*matrix)[3][2] = -1.0f;
    (*matrix)[3][3] = 0.0f;
}

void RenderSystem::Setup2DElementDraw(void) {
    Mtx mtx;
    DEBUGASSERTLINE(262, m_curMode & kRenderModeDraw);
    if (!(m_curMode & kRenderModeDraw2D)) {
        m_curMode |= kRenderModeDraw2D;
        GXSetProjection(m_mtx1, GX_ORTHOGRAPHIC);
        MTXIdentity(mtx);
        GXLoadPosMtxImm(mtx, 0);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetZMode(false, GX_ALWAYS, false);
    }
}

RenderSystem::RenderSystem() {
    float local0;
    float local4;

    memset(&m_RenderMode, 0, sizeof(m_RenderMode));
    m_pRenderMode         = NULL;
    m_pFirstFramebuffer   = NULL;
    m_pSecondFramebuffer  = NULL;
    m_pCurrentFramebuffer = NULL;
    m_curMode             = 0;
    local0                = 0.00390625f;
    local4                = 0.0044642859f;
    MTXIdentity(m_mtx1);
    m_mtx1[0][0] = local0;
    m_mtx1[1][1] = local4;
    m_mtx1[0][3] = -1.0f;
    m_mtx1[1][3] = 1.0f;
    UpdateShadowCameraMatrix(&m_shadowCameraMatrix, 45.0f, 4.0f / 3.0f, 0.01f, 1024.0f);
    MTXIdentity(m_mtx3);
}

RenderSystem::~RenderSystem() {}

void RenderSystem::SetupTextureDrawIn3DSpace() {
    GXSetZCompLoc(true);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetNumTexGens(1);
    GXSetNumChans(1);
    GXSetNumTevStages(1);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_AND);
    GXSetChanCtrl(GX_COLOR0A0, false, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
}
