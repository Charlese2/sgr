#include "game/sound_3d.h"
#include "game/snd.h"

extern sound_3d SndInstances[MAX_SND_INSTANCES];
extern sound_3d AmbientSounds[MAX_AMBIENT_SOUNDS];

class sound_header {
    char header[32];
};

void InitializeSoundSystem();
void ReinitializeSoundSystems(void);
void SetupSoundEffect(sound_3d *soundEffect);
sound_header *GetSoundHeader(char *filename);
