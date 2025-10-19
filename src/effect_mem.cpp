#include "game/effect_mem.h"

char string_buffer[512];

char *Types[8] = {
    "preload",
    "cutscene",
    "level",
    "spell",
    "character",
    "summon",
    "levelchar",
    ""
};

void effects_mem_info::initialize_info() {
    m_type = Types;
}
