#include "game/memory.h"

class characterinfo {
  public:
    characterinfo() {};
    Mempool *GetMempoolForSpecies();
    void LoadCharacterInfo();
    static bool character_info_is_main_player_species(int species);

    int unk0;

  private:
    Mempool mempool;
    Mempool mempool2;
    int species;
    char unk[0x27F00];
};

class statscreen {
    char unk[0x32000];
};

class characterinfo_something {
    char unk[0x45d30];
};

void DisplayCharacterMeminfo(char *buffer);
