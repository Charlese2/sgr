#include "dolphin/types.h"

#define MAX_TARGETS 1200
#define BM_BOGUS_BITMAP_HANDLE 0

class gr_ngc {
  public:
    static void BitmapInfo(int bitmapId, u32 &bmpWidth, u32 &bmpHeight);
    static void SetScissor(u32 left, u32 top, u32 width, u32 height);
    static void set_z_mode(int mode);
    static void DrawStaticTexture(int *bmpHandle, u32 unk2, u32 unk3, int unk4, int unk5, int unk6, int unk7, int unk8, int unk9,
                                  int unk10);
    static void DrawDynamicTexture(s16 start_x_pixel, s16 start_y_pixel, s16 end_x_pixel, s16 end_y_pixel);
    static int FindTargetFromFilename(const char *filename, char **target_filename);
    static void set_alpha_blending_mode(int mode);
    static void CalculateScissor();
};
