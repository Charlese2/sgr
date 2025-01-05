#include "dolphin/axart.h"

void AXARTPitchEnv(AXART_PitchEnv * articulator) {
    if (articulator->cents != articulator->target) {
        articulator->cents += articulator->delta;
        if (articulator->delta < 0 && articulator->cents > articulator->target) {
            articulator->cents = articulator->target;
        }
    }

}

void AXARTVolumeEnv(AXART_VolumeEnv * articulator) {
    if (articulator->attenuation != articulator->target) {
        articulator->attenuation += articulator->delta;
        if (articulator->delta < 0 && articulator->attenuation > articulator->target) {
            articulator->attenuation = articulator->target;
        }
    }
}
