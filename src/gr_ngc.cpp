#include "game/debug.h"
#include "dolphin/gx.h"
#include "game/macros.h"

void set_alpha_blending_mode(int mode) {
    switch (mode) {
        case 0:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
            break;
        case 1:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
            break;
        case 2:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_SET);
            break;
        case 3:
            GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR);
            break;
        default:
            DEBUGERRORLINE(746, "Unsupported alpha blending mode");
    }
}
