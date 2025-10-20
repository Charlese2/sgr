#include "game/memcard.h"
#include "game/effect_mem.h"
#include "game/interface.h"
#include "game/FileSystem.h"
#include "game/memory.h"

u8 card_work_area[40960];
CrankyMemcard memorycard;
int CardId;

int memcard::DoFormat(void) {
    char *cardSlotName;
    if (CardId == 0) {
        cardSlotName = "Slot A";
    } else {
        cardSlotName = "Slot B";
    }
    sprintf(string_buffer, "Formatting %s. Do not remove the %s in %s, reset, or switch off the console.", "Memory Card", "Memory Card",
            cardSlotName);
    SetActiveSlot(CardId);
    GetState(NULL);
    if (IsMounted()) {
        Format(NULL);
        Unmount();
    }
    return 0;
}

void FormatDialogueCallback(int state) {}

void memcard::ShowFormatDialogue() {
    char *cardSlotName;
    if (CardId == 0) {
        cardSlotName = "Slot A";
    } else {
        cardSlotName = "Slot B";
    }
    switch (memorycard.GetState(NULL)) {

    case 0:
        sprintf(string_buffer, "The %s in %s is damaged and cannot be used.", "Memory Card", cardSlotName);
    case 1:
        sprintf(string_buffer, "The %s in %s is damaged and cannot be used.", "Memory Card", cardSlotName);
    case 2:
        sprintf(string_buffer, "No %s found. You will be unable to save your progress without a %s.", "Memory Card", "Memory Card");
    case 3:
        sprintf(string_buffer, "The %s in %s is corrupted and needs to be formatted.", "Memory Card", cardSlotName);
    case 4:
        sprintf(string_buffer, "The %s in %s is formatted for another market. To save a Summoner file, you will need to format the %s.",
                "Memory Card", cardSlotName, "Memory Card");
    case 5:
        sprintf(string_buffer, "Wrong device in %s.  Please insert a %s.", cardSlotName, "Memory Card");
    case 7:
        sprintf(string_buffer, "The %s in %s is not compatible with the Summoner save file.", "Memory Card", cardSlotName);
    }
    ShowDialogue(string_buffer, -1, 0, 0, &FormatDialogueCallback, 18);
}

void GetSaveIcon(u32 *icon) {
    u32 *iconBuffer = allocate_in_commonblock(2079);
    CrankyFile iconFile;
    int size;
    iconFile.OpenFile(FileSystem::GetFilePath(0), "s2-mcicon.imd");
    size = iconFile.GetFileSize();
    DEBUGASSERTLINE(286, size == kMemcardIconSizeRGB5A3);
    iconFile.ReadFile(iconBuffer, kMemcardIconSizeRGB5A3);
    iconFile.CloseFile();
    memcpy(icon, iconBuffer, kMemcardIconSizeRGB5A3);
    deallocate_from_commonblock(iconBuffer);
}
