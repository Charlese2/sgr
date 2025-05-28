#include "game/ImageHeader.h"
#include "dolphin/os.h"

ImageHeader::ImageHeader() {}

ImageHeader::~ImageHeader() {}

void ImageHeader::Unknown() {
    if (data) {
        this->data = (u32 *)((int)data + (int)&data);
        ASSERTMSGLINE(56, ((u32)data & 0x1f) == 0, "non 32-byte aligned data pointer in texture palette");
    }
}
