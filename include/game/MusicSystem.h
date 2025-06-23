#include "dolphin/dvd.h"
#include "dolphin/types.h"

class MusicSystem {
  public:
    void LoadMusic(void);
    void PlayMusic(void);
    void SetMusicVolume(u8 volume);
    u8 GetMusicVolume() const { return m_Volume; };

  private:
    char music_path[32];
    u32 *unk20;
    u32 *unk24;
    u32 *unk28;
    u32 unk2c;
    void *callback;
    u8 unk34;
    u8 unk35;
    u8 unk36;
    u8 unk37;
    u8 unk38;
    u8 unk39;
    u8 unk3a;
    u8 unk3b;
    u32 unk3c;
    DVDCommandBlock *unk40;
    u32 unk44;
    u32 unk48;
    u8 unk4c;
    u8 unk4d;
    u8 unk4e;
    u8 unk4f;
    u8 unk50;
    u8 unk51;
    u8 unk52;
    u8 unk53;
    u8 unk54;
    u8 unk55;
    u8 unk56;
    u8 unk57;
    u8 unk58;
    u8 unk59;
    u8 unk5a;
    u8 unk5b;
    u32 *unk5c;
    u8 unk60;
    u8 unk61;
    u8 unk62;
    u8 unk63;
    u32 startAddr;
    u32 length;
    DVDCallback *DVDCallback;
    char unk70[64];
    u8 unkb0[48];
    bool unke0;
    bool unke1;
    bool unke2;
    u8 unke3;
    u8 m_Volume;
    bool unke5;
    bool unke6;
    bool unke7;
};

extern MusicSystem gMusicSystem;
