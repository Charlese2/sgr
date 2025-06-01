#include "game/sound_3d.h"

extern sound_3d SoundEffects[96];
extern sound_3d Ambience[64];

void InitializeSoundSystem();
void ReinitializeSoundSystems(void);
void SetupSoundEffect(sound_3d *soundEffect);
