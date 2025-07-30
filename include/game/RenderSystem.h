#include "game/new_math.h"
#include "dolphin/gx/GXStruct.h"
#include "dolphin/mtx.h"
#include "dolphin/types.h"
#include "game/vector.h"

#define kRenderModeDraw   1
#define kRenderModeDraw2D 2

#define NGPS_HEIGHT 448

class RenderSystem {
  public:
    virtual ~RenderSystem();
    RenderSystem();
    static void UpdateShadowCameraMatrix(Mtx44 matrix, float field_of_view, float aspect_ratio, float unk3, float unk4);
    void SetShadowPositionMatrix(vector3x3 *unk1, vector3 *unk2);
    void GetShadowPositionMatrixCopy(Mtx shadowPositionMatrix);
    void GetShadowCameraMatrixCopy(Mtx44 shadowCameraMatrix);
    void SomethingRenderMode(void);
    u32 GetCurrentMode(u32 flag) { return flag & m_curMode; }
#ifdef DEBUG
    void Setup2DElementDraw(void);
#else
    void Setup2DElementDraw(bool force);
#endif // DEBUG

    void InitRenderMode(GXRenderModeObj *pRenderMode);
    void InitFramebuffers(void);
    void InitGX(void);
    void DrawFirstFramebuffer(void);
    void Unknown(void);
    void SetupUnknownDraw(void);
    void Unknown2(void);
    void SwapFramebuffers(void);
    void SetupTextureDrawIn3DSpace(void);
    void Unknown3(bool unk);
    void Unknown4(bool unk);
    void Unknown5(bool unk);
    void Unknown6(float alpha, bool unk);
    void Unknown7(void);
    static void DiagnoseHang();
    static void SetGPHangMetric(u8 enable);
    static void EnableGPHangWorkaround(u32 timeoutFrames);
    static void NoHangRetraceCallback(u32 count);
    static void NoHangDoneRender(u16 token);
    static void ReInit(GXRenderModeObj* mode);
    static void InitVI();
    static void RenderFrame();

  private:
    u32 m_curMode;
    u8 m_firstFrame;
    GXRenderModeObj *m_pRenderMode;
    GXRenderModeObj m_RenderMode;
    void *m_pFirstFramebuffer;
    void *m_pSecondFramebuffer;
    void *m_pCurrentFramebuffer;
    Mtx44 m_mtx1;
    Mtx44 m_shadowCameraMatrix;
    Mtx m_shadowPositionMatrix;
    int unk108;
};

extern RenderSystem gRenderSystem;
