#include "dolphin/axart.h"

static f32 __AXART3DDopplerScale;
static f32 __AXART3DDistanceScale;

void AXARTSet3DDopplerScale(f32 scale) {
    __AXART3DDopplerScale = scale;
}

void AXARTSet3DDistanceScale(f32 scale) {
    __AXART3DDistanceScale = scale;
}

AXART_3DSound AXART3DSound(AXART_3DSound *unk) {
    
}
