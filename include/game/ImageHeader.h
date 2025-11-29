#ifndef _IMAGE_HEADER_H_
#define _IMAGE_HEADER_H_

#include "dolphin/types.h"

class ImageHeader {
    u32 *unk0;
    int unk4;
    u32 *data;

  public:
    ImageHeader();
    ~ImageHeader();
    void Unknown();
    void GetImageDimentions(u16 &bmpHeight, u16 &bmpWidth);
};

#endif //_IMAGE_HEADER_H_
