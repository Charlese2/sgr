#include "game/object.h"

extern bool Players_immortal;

class living_entity : object {
  public:
    living_entity();
    ~living_entity();
    void give_status(int unk1, int unk2, int unk3, float duration, int unk4, bool unk5);

  private:
    char unk[1382];
};

extern living_entity *Main_entity;
