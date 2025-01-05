#ifndef _DOLPHIN_AXART_H_
#define _DOLPHIN_AXART_H_

#include "dolphin/ax.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void * next;
    void * prev;
    AXVPB* axvpb;
    f32 sampleRate;
    void* articulators;
} AXARTSound;

typedef struct {
    f32 * lfo;
    u32 length;
    f32 delta;
    u32 sampleIndex;
    f32 counter;
    f32 sample1;
    f32 sample;
    f32 output;
} AXART_Lfo;

typedef struct {
    void * next;
    u32 type;
} AXArticulator;

typedef struct {
    AXArticulator art;
    f32 hAngle;
    f32 vAngle;
    f32 dist;
    f32 closingSpeed;
    u32 update;
    u8 pan;
    u8 span;
    u8 src;
    u16 itdL;
    u16 itdR;
    f32 pitch;
    s32 attenuation;
} AXART_3DSound;

typedef struct {
    AXArticulator art;
    u8 pan;
    u8 span;
} AXART_Pan;

typedef struct {
    AXArticulator art;
    u16 itdL;
    u16 itdR;
} AXART_Itd;

typedef struct {
    AXArticulator art;
    u8 src;
} AXART_Srctype;

typedef struct {
    AXArticulator art;
    s32 cents;
} AXART_Pitch;

typedef struct {
    AXArticulator art;
    s32 delta;
    s32 target;
    s32 cents;
} AXART_PitchEnv;

typedef struct {
    AXArticulator art;
    AXART_Lfo lfo;
    s32 cents;
} AXART_PitchMod;

typedef struct {
    AXArticulator art;
    s32 attenuation;
} AXART_Volume;

typedef struct {
    AXArticulator art;
    s32 delta;
    s32 target;
    s32 attenuation;
} AXART_VolumeEnv;

typedef struct {
    AXArticulator art;
    AXART_Lfo lfo;
    s32 attenuation;
} AXART_VolumeMod;

typedef struct {
    AXArticulator art;
    u32 initLPF;
    u32 frequency;
    u32 update;
} AXART_Lpf;


// axart.c
void AXARTInit();
void AXARTServiceSounds();
void AXARTInitSound(AXARTSound * sound, AXVPB * voice, u32 sampleRate);
void AXARTAddSound(AXARTSound * sound);
void AXARTRemoveSound(AXARTSound * sound);
void AXARTInitLfo(AXART_Lfo * lfo, f32 samples, u32 length, f32 delta);
void AXARTInitArt3D(AXART_3DSound * articulator);
void AXARTInitArtPanning(AXART_Pan * articulator);
void AXARTInitArtItd(AXART_Itd * articulator);
void AXARTInitArtSrctype(AXART_Srctype * articulator);
void AXARTInitArtPitch(AXART_Pitch * articulator);
void AXARTInitArtPitchEnv(AXART_PitchEnv * articulator);
void AXARTInitArtPitchMod(AXART_PitchMod * articulator);
void AXARTInitArtVolume(AXART_Volume * articulator);
void AXARTInitArtAuxAVolume(AXART_Volume * articulator);
void AXARTInitArtAuxBVolume(AXART_Volume * articulator);
void AXARTInitArtVolumeEnv(AXART_VolumeEnv * articulator);
void AXARTInitArtAuxAVolumeEnv(AXART_VolumeEnv * articulator);
void AXARTInitArtAuxBVolumeEnv(AXART_VolumeEnv * articulator);
void AXARTInitArtVolumeMod(AXART_VolumeMod * articulator);
void AXARTInitArtAuxAVolumeMod(AXART_VolumeMod * articulator);
void AXARTInitArtAuxBVolumeMod(AXART_VolumeMod * articulator);
void AXARTInitArtLpf(AXART_Lpf * articulator);

// axartsound.c
void AXARTServiceSound(AXARTSound * sound);
void AXARTAddArticulator(AXARTSound * sound, AXArticulator * articulator);

// axartcents.c
f32 AXARTCents(s32 cents);

// axartenv.c
void AXARTPitchEnv(AXART_PitchEnv * articulator);
void AXARTVolumeEnv(AXART_VolumeEnv * articulator);

// axartlfo.c
void AXARTLfo(AXART_Lfo * lfo);

// axart3d.c
void AXARTSet3DDopplerScale(f32 scale);
void AXARTSet3DDistanceScale(f32 scale);
AXART_3DSound AXART3DSound(AXART_3DSound * articulator);

#ifdef __cplusplus
};
#endif

#endif // _DOLPHIN_AXART_H_
