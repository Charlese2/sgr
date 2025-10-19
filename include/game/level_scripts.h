#include "game/vector.h"
#define MAX_LEVEL_SCRIPT_NAVPOINTS          700
#define MAX_LEVEL_SCRIPT_CONSTRAIN_EFFECTS  32
#define MAX_LEVEL_LOAD_SPLINES_PER_LEVEL    16
#define MAX_LEVEL_ITEMS_PER_LEVEL           16
#define MAX_USE_ITEM_SEQUENCES_PER_CREATURE 16
#define MAX_LE_USE_ITEM_SEQUENCES           64
#define MAX_DEATH_SEQUENCES                 64
#define MAX_DETECT_SEQUENCES                64
#define MAX_CLICKS_PER_LEVEL                64
#define MAX_LEVEL_LOAD_SPLINES_PER_LEVEL    16
#define MAX_SHOPKEEPERS_PER_LEVEL           15
#define MAX_TRIGGER_SPLINES_PER_LEVEL       64
#define MAX_TRIGGER_SPHERES_PER_LEVEL       32
#define MAX_REGIONS_PER_LEVEL               10
#define MAX_QUADS_PER_REGION                4
#define MAX_NAVPOINT_NAME_LEN               16

class script_navpoint {
  public:
    char m_navpoint_name[MAX_NAVPOINT_NAME_LEN];
    vector3x3 unk10;
    vector3 unk34;
    int unk40;
    int unk44;
    int unk48;
    int unk4C;

    static script_navpoint* get_script_navpoint_by_name(char * name);
};
