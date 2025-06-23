#include "game/snd.h"
#include "game/sound_3d.h"

class volume_info {
  public:
    float m_soundVolume;
    float m_voiceVolume;
    float m_musicVolume;
};

extern volume_info volumeInfo;

class sound_header {
    char header[32];
};

class SoundConvert {
  public:
    static void InitializeSoundSystem();
    static void ReinitializeSoundSystems(void);
    static void SetupSoundEffect(sound_3d *soundEffect);
    static float GetSoundVolume(int type) { return volumeInfo.m_soundVolume; };
    static sound_header *GetSoundHeader(char *filename);
};
