typedef struct {
    int unk0;
    int unk4;
    int type;

} item_info;

typedef struct {
    int unk0;
    item_info* info;
} inventory_item;

class NewInventory {
    bool is_item_equippable(inventory_item item, bool unk);
};

class inv {
    char unk[0x1fa18];
};
