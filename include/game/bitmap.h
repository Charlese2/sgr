#include "dolphin/types.h"
#include "game/ImageHeader.h"

class Bitmap {
  public:
    static ImageHeader &GetFromId(int bitmapId);

    u32 m_width;
    u32 m_height;
};
