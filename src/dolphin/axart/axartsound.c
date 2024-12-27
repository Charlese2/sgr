#include "dolphin/axart.h"

void AXARTServiceSound(AXARTSound *sound) {

}

void AXARTAddArticulator(AXARTSound *sound, AXARTArticulator *articulator) {
    articulator->next = sound->articulators;
    sound->articulators = articulator;
}
