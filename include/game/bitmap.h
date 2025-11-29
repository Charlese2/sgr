#ifndef _BITMAP_H_
#define _BITMAP_H_

#include "dolphin/types.h"
#include "game/ImageHeader.h"

class Bitmap {
  public:
    ImageHeader &GetImageHeader();

    ImageHeader *m_header;
    int unk;
    u16 m_width;
    u16 m_height;
};

#endif // _BITMAP_H_
