#include "game/CrankyMemcard.h"
#include "dolphin/card.h"
#include "global.h"

CrankyMemcard::CrankyMemcard() {

}

CrankyMemcard::~CrankyMemcard() {
    ASSERTLINE(47, !m_isMounted);
}

void CrankyMemcard::NewMemcard(int* cardWorkArea) {
    CARDInit();
    m_cardWorkArea = cardWorkArea;
}
