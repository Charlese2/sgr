#include "dolphin/axart.h"

void AXARTServiceSound(AXARTSound * sound) {

}

void AXARTAddArticulator(AXARTSound * sound, AXArticulator * articulator) {
    articulator->next = sound->articulators;
    sound->articulators = articulator;
}
