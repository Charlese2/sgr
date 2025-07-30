#include "game/RenderSystem.h"
extern "C" {
#include "dolphin/gx.h"
#include "dolphin/vi.h"
#include "dolphin/mtx.h"
#include "dolphin/os.h"
}
#include "game/macros.h"

RenderSystem gRenderSystem;

void RenderSystem::UpdateShadowCameraMatrix(Mtx44 matrix, float field_of_view, float aspect_ratio, float near_clip, float far_clip) {
    float cot;
    float angle;
    float tmp;

    ASSERTMSGLINE(41, matrix, "no matrix present");
    ASSERTMSGLINE(42, field_of_view > 0.0 && field_of_view < 180.0, "invalid field-of-view");
    ASSERTMSGLINE(43, aspect_ratio != 0.0f, "invalid aspect ratio");
    angle        = field_of_view * DEG_TO_RAD(1);
    angle        = 0.5f * angle;
    cot          = 1.0f / Math::tanf(angle);
    matrix[0][0] = -(cot / aspect_ratio);
    matrix[0][1] = 0.0f;
    matrix[0][2] = 0.0f;
    matrix[0][3] = 0.0f;
    matrix[1][0] = 0.0f;
    matrix[1][1] = cot;
    matrix[1][2] = 0.0f;
    matrix[1][3] = 0.0f;
    matrix[2][0] = 0.0f;
    matrix[2][1] = 0.0f;
    tmp          = 1.0f / (far_clip - near_clip);
    matrix[2][2] = -near_clip * tmp;
    matrix[2][3] = -(far_clip * near_clip) * tmp;
    matrix[3][0] = 0.0f;
    matrix[3][1] = 0.0f;
    matrix[3][2] = -1.0f;
    matrix[3][3] = 0.0f;
}

void RenderSystem::SetShadowPositionMatrix(vector3x3 *unk1, vector3 *unk2) {
    m_shadowPositionMatrix[0][0] = unk1->vec1.x;
    m_shadowPositionMatrix[0][1] = unk1->vec1.y;
    m_shadowPositionMatrix[0][2] = unk1->vec1.z;
    m_shadowPositionMatrix[1][0] = unk1->vec2.x;
    m_shadowPositionMatrix[1][1] = unk1->vec2.y;
    m_shadowPositionMatrix[1][2] = unk1->vec2.z;
    m_shadowPositionMatrix[2][0] = -unk1->vec3.x;
    m_shadowPositionMatrix[2][1] = -unk1->vec3.y;
    m_shadowPositionMatrix[2][2] = -unk1->vec3.z;
    m_shadowPositionMatrix[0][3] = -VECDotProduct((Vec *)unk2, (Vec *)&unk1->vec1);
    m_shadowPositionMatrix[1][3] = -VECDotProduct((Vec *)unk2, (Vec *)&unk1->vec2);
    m_shadowPositionMatrix[2][3] = VECDotProduct((Vec *)unk2, (Vec *)&unk1->vec3);
}

void RenderSystem::GetShadowPositionMatrixCopy(Mtx shadowPositionMatrix) { MTXCopy(m_shadowPositionMatrix, shadowPositionMatrix); }

void RenderSystem::GetShadowCameraMatrixCopy(Mtx44 shadowCameraMatrix) { MTX44Copy(m_shadowCameraMatrix, shadowCameraMatrix); }

void RenderSystem::SomethingRenderMode() {
    DEBUGASSERTLINE(233, m_curMode & kRenderModeDraw);
    if (m_curMode & kRenderModeDraw2D) {
        m_curMode &= ~kRenderModeDraw2D;
        GXSetProjection(m_shadowCameraMatrix, GX_PERSPECTIVE);
        GXSetZMode(TRUE, GX_LEQUAL, TRUE);
    }
}

#ifdef DEBUG
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
        GXSetZMode(FALSE, GX_ALWAYS, FALSE);
    }
}
#else
void RenderSystem::Setup2DElementDraw(bool force) {
    Mtx mtx;
    DEBUGASSERTLINE(262, m_curMode & kRenderModeDraw);
    if (force || !(m_curMode & kRenderModeDraw2D)) {
        m_curMode |= kRenderModeDraw2D;
        GXSetProjection(m_mtx1, GX_ORTHOGRAPHIC);
        MTXIdentity(mtx);
        GXLoadPosMtxImm(mtx, 0);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetZMode(FALSE, GX_ALWAYS, FALSE);
    }
}
#endif

RenderSystem::RenderSystem() {
    float local0;
    float local4;

    memset(&m_RenderMode, 0, sizeof(m_RenderMode));
    m_pRenderMode         = NULL;
    m_pFirstFramebuffer   = NULL;
    m_pSecondFramebuffer  = NULL;
    m_pCurrentFramebuffer = NULL;
    m_firstFrame          = 1;
    m_curMode             = 0;
    local0                = 1.0f / 256.0f;
    local4                = 1.0f / 224.0f;
    MTXIdentity(m_mtx1);
    m_mtx1[0][0] = local0;
    m_mtx1[1][1] = local4;
    m_mtx1[0][3] = -1.0f;
    m_mtx1[1][3] = 1.0f;
    UpdateShadowCameraMatrix(m_shadowCameraMatrix, 45.0f, (4.0f / 3.0f), 0.01f, 1024.0f);
    MTXIdentity(m_shadowPositionMatrix);
}

RenderSystem::~RenderSystem() {}

void RenderSystem::InitRenderMode(GXRenderModeObj *pRenderMode) {
    if (pRenderMode) {
        m_RenderMode  = *pRenderMode;
        m_pRenderMode = &m_RenderMode;
    } else {
        switch (VIGetTvFormat()) {
        case VI_NTSC:
            m_pRenderMode = &GXNtsc480IntDf;
            break;
        case VI_PAL:
            m_pRenderMode = &GXPal528IntDf;
            break;
        case VI_EURGB60:
            m_pRenderMode = &GXEurgb60Hz480IntDf;
            break;
        case VI_MPAL:
            m_pRenderMode = &GXMpal480IntDf;
            break;
        default:
#ifdef DEBUG
            OSPanic(__FILE__, 364, "DEMOInit: invalid TV format\n");
#else
            OSPanic(__FILE__, 388, "DEMOInit: invalid TV format\n");
#endif
            break;
        }
        GXAdjustForOverscan(m_pRenderMode, &m_RenderMode, 0, 16);
        m_pRenderMode        = &m_RenderMode;
        m_RenderMode.fbWidth = 512;
    }
    VIConfigure(m_pRenderMode);
    m_firstFrame = 1;
    DEBUGASSERTLINE(380, NGPS_HEIGHT == m_pRenderMode->xfbHeight);
}

void RenderSystem::InitFramebuffers(void) {
    void *arenaLo;

    arenaLo    = OSGetArenaLo();
    u32 fbSize = (u16)(((u16)m_pRenderMode->fbWidth + 15) & 0xFFFFFFF0) * m_pRenderMode->xfbHeight * 2;

    m_pFirstFramebuffer   = (void *)OSRoundUp32B(arenaLo);
    m_pSecondFramebuffer  = (void *)OSRoundUp32B((u32)m_pFirstFramebuffer + (u32)fbSize);
    m_pCurrentFramebuffer = m_pSecondFramebuffer;

    arenaLo = (void *)OSRoundUp32B((u32)m_pSecondFramebuffer + fbSize);
    OSSetArenaLo(arenaLo);
}

void RenderSystem::InitGX() {
    u32 yScaleFactor;
    u16 yScale;

    GXSetViewport(0.0f, 0.0f, m_pRenderMode->fbWidth, m_pRenderMode->efbHeight, 0.0f, 1.0f);
    GXSetScissor(0, 0, m_pRenderMode->fbWidth, m_pRenderMode->efbHeight);
    yScaleFactor = GXGetYScaleFactor(m_pRenderMode->efbHeight, m_pRenderMode->xfbHeight);
    yScale       = GXSetDispCopyYScale(yScaleFactor);
    GXSetDispCopySrc(0, 0, m_pRenderMode->fbWidth, m_pRenderMode->efbHeight);
    GXSetDispCopyDst(m_pRenderMode->fbWidth, yScale);
    GXSetCopyFilter(m_pRenderMode->aa, m_pRenderMode->sample_pattern, GX_TRUE, m_pRenderMode->vfilter);
    if (m_pRenderMode->aa) {
        GXSetPixelFmt(GX_PF_RGB565_Z16, GX_ZC_LINEAR);
    } else {
        GXSetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
    }
    GXCopyDisp(m_pCurrentFramebuffer, GX_TRUE);
#ifdef DEBUG
    GXSetVerifyLevel(GX_WARN_NONE);
#endif
    GXSetDispCopyGamma(GX_GM_1_0);
}

void RenderSystem::DrawFirstFramebuffer(void) {
    VISetNextFrameBuffer(m_pFirstFramebuffer);
    m_pCurrentFramebuffer = m_pSecondFramebuffer;
    VIFlush();
    VIWaitForRetrace();
    u32 bDsTvMode = m_pRenderMode->viTVmode & VI_TVMODE_NTSC_DS;
    if (bDsTvMode) {
        VIWaitForRetrace();
    }
}

void RenderSystem::Unknown() {
    GXColor clearColor = {};
    GXSetCopyClear(clearColor, 0xFFFFFF);
    GXSetCullMode(GX_CULL_BACK);
    GXSetScissor(0, 0, 512, 448);
    GXSetClipMode(GX_CLIP_ENABLE);
}
void RenderSystem::SetupUnknownDraw(void) {
    if (!(m_curMode & kRenderModeDraw)) {
        DEBUGASSERTLINE(515, !(m_curMode & kRenderModeDraw2D));
        m_curMode = m_curMode | kRenderModeDraw;
        if (m_pRenderMode->field_rendering) {
            GXSetViewportJitter(0.0f, 0.0f, m_pRenderMode->fbWidth, m_pRenderMode->efbHeight, 0.0f, 1.0f, VIGetNextField());
        } else {
            GXSetViewport(0.0f, 0.0f, m_pRenderMode->fbWidth, m_pRenderMode->efbHeight, 0.0f, 1.0f);
        }
        GXInvalidateVtxCache();
        GXInvalidateTexAll();
        GXSetProjection(m_shadowCameraMatrix, GX_PERSPECTIVE);
    }
}

void RenderSystem::Unknown2() {
    if ((m_curMode & kRenderModeDraw) == 0) {
        return;
    }
    if ((m_curMode & kRenderModeDraw2D) != 0) {
        SomethingRenderMode();
    }
    m_curMode &= ~kRenderModeDraw;
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetColorUpdate(GX_TRUE);
    GXCopyDisp(m_pCurrentFramebuffer, GX_TRUE);
    GXDrawDone();
    SwapFramebuffers();
}

void RenderSystem::SwapFramebuffers(void) {
    DEBUGASSERTLINE(595, !GetCurrentMode(kRenderModeDraw));
    DEBUGASSERTLINE(596, !GetCurrentMode(kRenderModeDraw2D));
    VISetNextFrameBuffer(m_pCurrentFramebuffer);
    if (m_firstFrame) {
        VISetBlack(0);
        m_firstFrame = 0;
    }
    VIFlush();
    VIWaitForRetrace();
    if (m_pCurrentFramebuffer == m_pFirstFramebuffer) {
        m_pCurrentFramebuffer = m_pSecondFramebuffer;
    } else {
        m_pCurrentFramebuffer = m_pFirstFramebuffer;
    }
}

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

void RenderSystem::Unknown3(bool unk) {
    GXSetZCompLoc(GX_TRUE);
    GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_GREATER, 0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetNumChans(1);
    if (unk) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_SET);
    } else {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    }
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
}

void RenderSystem::Unknown4(bool unk) {
    GXSetZCompLoc(GX_TRUE);
    GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_GREATER, 0);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetNumChans(1);
    if (unk) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    } else {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    }
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
}

void RenderSystem::Unknown5(bool unk) {
    GXSetZCompLoc(GX_FALSE);
    GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 5, GX_AOP_AND, GX_GREATER, 5);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetNumTexGens('\x01');
    GXSetNumTevStages('\x01');
    GXSetNumChans('\x01');
    if (unk) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    } else {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    }
    GXSetChanCtrl(GX_COLOR0A0, '\0', GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, '\0');
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, '\0');
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, '\0');
}

void RenderSystem::Unknown6(float alpha, bool unk) {
    GXColor color;
    GXSetZCompLoc(GX_FALSE);
    GXSetZMode(GX_TRUE, GX_LESS, GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 5, GX_AOP_AND, GX_GREATER, 5);
    GXSetNumTexGens(1);
    GXSetNumTevStages(3);
    GXSetNumChans(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60);
    color.r = color.g = color.b = 0;
    color.a                     = 255.5f * alpha;
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_TEXC, GX_CC_CPREV, GX_CC_A0, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_TEXA, GX_CA_APREV, GX_CA_A0, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASA, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    if (unk) {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    } else {
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    }
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, '\0');
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, '\0');
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, '\0');
}

void RenderSystem::Unknown7() {
    GXCopyDisp(m_pCurrentFramebuffer, GX_TRUE);
    GXDrawDone();
    GXCopyDisp(m_pFirstFramebuffer, GX_TRUE);
    GXDrawDone();
    GXCopyDisp(m_pSecondFramebuffer, GX_TRUE);
    GXDrawDone();
}
