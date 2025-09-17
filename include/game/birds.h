#include "game/vector.h"
#include "game/timestamp.h"

#define MAX_BIRDS 20

class bird {
    int unk0;
    vector3 unk4;
    vector3 unk10;
    vector3x3 unk1C;
    vector3x3 unk40;
    int unk64;
    float unk68;
    vector3 unk6C;
    GameTimer unk78;
  public:

    static void Render(void);
    static void Unknown(void);
    static void Unknown2(char *type, int num_birds);
    static void Unknown3(void);
    static void ResetCount(void);
    static void Unknown4(int birdnum);
    static void Unknown5(int birdnum, vector3* unk);
    static void Unknown6(int birdnum);
};
