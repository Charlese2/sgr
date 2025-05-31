class vector3 {
  public:
    float x;
    float y;
    float z;
    vector3(){};
};

class pathfinding_transform {
  public:
    vector3 unk0;
    int unkC;
    vector3 unk10;
    int unk1C;
    pathfinding_transform *next;
    pathfinding_transform *prev;
    pathfinding_transform(){};
};

class effect_transform {
  public:
    int unk0;
    int unk4;
    vector3 unk8;
    vector3 unk14;
    int unk20;
    int unk24;
};
