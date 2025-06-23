class font_face {

};

class gr_font {
  public:
    static int GetFontHeight();

    u32 loaded_fonts;
    font_face *font_face;
    s32 current_width_offset;
    s32 current_height_offset;
    int unk14;
};
