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
    static bool init(int width, int height, int unk3, int window_mode, bool unk5, bool unk6);
    static void DrawScreenFade(void);
    static void SetBounds(int left, int top, int width, int height);
    static void CalculateScreenBounds(void);
    static void DrawLoadingScreenBackground(void);
    static void DrawScreen(void);
    static void SetDynamicTextureColor(u32 red, u32 green, u32 blue, u32 alpha);
    static void DrawHudColor(Color *color);
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
    int unk28;
    int unk2C;
    int unk30;
    int unk34;
    int unk38;
    int unk3C;
    int m_left;
    int unk44;
    int m_top;
    int unk4C;
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
