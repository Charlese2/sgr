#include "game/bitmap.h"
#include "game/RenderSystem.h"

class Color {
  public:
    Color() {}
    Color(s32 red, s32 green, s32 blue, s32 alpha);
    u8 red;
    u8 green;
    u8 blue;
    u8 alpha;
};

class gr {
  public:
    static int toggle_gr_show_stats(void);
    static int toggle_gr_show_paging(void);
    static int set_gr_ngps_hires(void);
    static void Unknown1(void);
    static bool init(int width, int height, int unk3, int window_mode, u32 unk5, bool unk6);
    static void Unknown3(bool unk);
    static void Unknown4(int unk);
    static bool Unknown5(void);
    static void Unknown6(int unk);
    static void Unknown7(int unk);
    static float GetAlpha(void);
    static void DrawScreenFade(void);
    static void GetBounds(int &left, int &top, int &width, int &height);
    static void SetBounds(int left, int top, int width, int height);
    static void CalculateScreenBounds(void);
    static void DrawLoadingScreenBackground(void);
    static void DrawScreen(void);
    static void CreateTables(float nth_root);
    static void SetDynamicTextureColor(s32 red, s32 green, s32 blue, s32 alpha);
    static void ResetDynamicTextureColor();
    static void DrawHudColor(const Color &color);
    static void SetDynamicTextureColorAlpha(u32 alpha);
    static void Unknown2(int unk1, int unk2);
    static void DrawStaticTexture(Bitmap *bmpHandle, u32 x, u32 y, int width, int height, int bmp_section_x, int bmp_section_y,
                                  int bmp_section_width, int bmp_section_height);
    static void DrawImage(Bitmap *bmpHandle, int x, int y);
    static void DrawLine2D(float x1, float y1, float x2, float y2);
    static void DrawBlackBackground(void);
    static void rect(s32 start_x_position, s32 start_y_position, s32 end_x_position, s32 end_y_position);
    static bool getInitialized(void);

    int unk0;
    int m_width;
    int m_height;
    int unkC;
    int m_window_mode;
    int unk14;
    float unk18;
    int unk1C;
    int unk20;
    int unk24;
    int m_left_bound;
    int m_top_bound;
    int m_width_bound;
    int m_height_bound;
    int unk38;
    int unk3C;
    int m_left;
    int m_drawable_width;
    int m_top;
    int m_drawable_height;
    Color m_dynamic_texture_color;
    int unk54;
    int unk58;
    int unk5C;
    int unk60;
    int unk64;
    int unk68;
    bool unk6C;
    int unk70;
};

extern gr gGr;
