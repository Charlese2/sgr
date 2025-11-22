#include "dolphin/types.h"

#define MAX_TARGETS 1200

class gr_ngc {
  public:
    static void set_z_mode(int mode);
    static void SetScissor(u32 left, u32 top, u32 width, u32 height);
    static void DrawStaticTexture(int *bmpHandle, u32, u32, int, int, int, int, int, int);
    static void DrawDynamicTexture(s16 start_x_pixel, s16 start_y_pixel, s16 end_x_pixel, s16 end_y_pixel);
    static int FindTargetFromFilename(const char *filename, char **target_filename);
    static void set_alpha_blending_mode(int mode);
    static void CalculateScissor();
};
