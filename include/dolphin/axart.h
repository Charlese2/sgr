#ifndef _DOLPHIN_AXART_H_
#define _DOLPHIN_AXART_H_

#include "dolphin/ax.h"


typedef struct {
    void * next;
    void * prev;
    AXVPB* axvpb;
    float sampleRate;
    void* unk10;
} AXARTSound;

extern void AXARTServiceSound(AXARTSound* p);

void AXARTServiceSounds();
void AXARTInitSound(AXARTSound * sound, AXVPB* p, float unk2);
void AXARTAddSound(AXARTSound * sound);
void AXARTRemoveSound(AXARTSound* sound);

#endif // _DOLPHIN_AXQUEUE_H_
