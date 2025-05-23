#include "game/level_sequences.h"
#include "game/gamemem.h"
#include "game/parse.h"

int Num_flipboox_anim;

void load_flipbook(parse* file) {
    Mempool *current_mempool;
    current_mempool = Gamemem_info.GetPerlevelMempool();
    set_current_mempool(current_mempool);
    Num_flipboox_anim = 0;

}
