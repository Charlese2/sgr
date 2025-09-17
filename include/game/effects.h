#include "game/vector.h"
#include "game/timestamp.h"

#define MAX_EFFECTS_PER_LEVEL   64
#define MAX_EFFECT_NAME_LEN     32
#define MAX_MODAL_RFX_LIST_SIZE 10

extern int Num_level_effects;

class effect {
  public:
    int unk0;
    int unk4;
    int type;
    vector3x3 unkC;
    vector3 unk30;
    vector3 unk3C;
    int unk48;
    vector3 unk4C;
    vector3 unk58;
    vector3 unk64;
    char unk92;
    GameTimer unkE8;
    vector3 unkC8;

    void Unknown();
};
