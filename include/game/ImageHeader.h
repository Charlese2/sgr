#include "dolphin/types.h"

class ImageHeader {
    int *unk0;
    int unk4;
    u32 *data;

  public:
    ImageHeader();
    ~ImageHeader();
    void Unknown();
    void GetImageDimentions(u16 &bmpHeight, u16 &bmpWidth);
};
