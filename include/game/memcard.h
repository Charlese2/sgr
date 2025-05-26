#include "game/CrankyMemcard.h"

const int kMemcardIconSizeRGB5A3 = 2048;

class memcard : CrankyMemcard {
    int DoFormat(void);
    int ShowFormatDialogue(void);
};


void FormatDialogueCallback(int state);
u32 *GetSaveIcon(u32* icon);

STATIC_ASSERT(sizeof(memcard) == 0x24);
