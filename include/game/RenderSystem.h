#include "game/new_math.h"
#include "dolphin/gx/GXStruct.h"
#include "dolphin/mtx.h"
#include "dolphin/types.h"
#include "game/vector.h"

#define kRenderModeDraw   1
#define kRenderModeDraw2D 2

class RenderSystem {
  public:
    virtual ~RenderSystem();
    RenderSystem();
    static void UpdateShadowCameraMatrix(Mtx44 matrix, float field_of_view, float aspect_ratio, float unk3, float unk4);
    void SetShadowPositionMatrix(vector3x3 *unk1, vector3 *unk2);
    void GetShadowPositionMatrixCopy(Mtx shadowPositionMatrix);
    void GetShadowCameraMatrixCopy(Mtx44 shadowCameraMatrix);
    void SomethingRenderMode();
    u32 GetCurrentMode(u32 flag) { return flag & m_curMode; }
    void Setup2DElementDraw(void);
    void SetupTextureDrawIn3DSpace(void);

  private:
    u32 m_curMode;
    int unk8;
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
