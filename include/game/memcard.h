#include "game/CrankyMemcard.h"

const int kMemcardIconSizeRGB5A3 = 2048;

class memcard : CrankyMemcard {
    void Unknown();
    int DoFormat(void);
    void ShowFormatDialogue(void);
};

void FormatDialogueCallback(int state);
void GetSaveIcon(u32 *icon);

STATIC_ASSERT(sizeof(memcard) == 0x24);
