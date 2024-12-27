#ifndef _DOLPHIN_AXART_H_
#define _DOLPHIN_AXART_H_

#include "dolphin/ax.h"


typedef struct {
    void * next;
    void * prev;
    AXVPB* axvpb;
    f32 sampleRate;
    void* articulators;
} AXARTSound;

typedef struct {
    void * next;
    u32 unk4;
} AXARTArticulator;

typedef struct {
    void * next;
    int type;
    f32 unk8;
    f32 unkc;
    f32 unk10;
    f32 unk14;
    u32 unk18;
    void * unk1c;
    u16 unk20;
    u16 unk22;
    f32 unk24;
    f32 unk28;
} AXART_3DSound;


// axart.c
void AXARTServiceSounds();
void AXARTInitSound(AXARTSound * sound, AXVPB* p, float unk2);
void AXARTAddSound(AXARTSound * sound);
void AXARTRemoveSound(AXARTSound* sound);

// axartsound.c
void AXARTAddArticulator(AXARTSound * sound, AXARTArticulator * articulator);
void AXARTServiceSound(AXARTSound * sound);

// axartcents.c
f64 AXARTCents(int unk);

// axartenv.c
void AXARTPitchEnv(void * unk);
void AXARTVolumeEnv(void * unk);

// axartlfo.c
void AXARTLfo( void * unk);

// axart3d.c
void AXARTSet3DDopplerScale(f64 scale);
void AXARTSet3DDistanceScale(f64 scale);
AXART_3DSound AXART3DSound(AXART_3DSound * unk);

#endif // _DOLPHIN_AXQUEUE_H_
