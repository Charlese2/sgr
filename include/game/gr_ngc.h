#include "dolphin/types.h"

#define MAX_TARGETS 1200

int FindTargetFromFilename(const char *filename, char **target_filename);
void set_alpha_blending_mode(int mode);
void set_z_mode(int mode);
void DrawDynamicTexture(s16 start_x_pixel, s16 start_y_pixel, s16 end_x_pixel, s16 end_y_pixel);
