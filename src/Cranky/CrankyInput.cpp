#include "game/CrankyInput.h"

CrankyInput::CrankyInput() {}

CrankyInput::~CrankyInput() {}

void CrankyInput::Initialize(bool static_init) {
    m_pPadStatus = NULL;
    low = 0.0f;
    high = 0.0f;
    unk4 = 0.0f;
    unk8 = 0.0f;
    if (static_init) {
        PADInit();
        PADRead(m_PadStatus);
        m_pPadStatus = m_PadStatus;
        m_tick = 0;
        for (int contId = 0; contId < PAD_MAX_CONTROLLERS; contId++) {
            if (m_PadStatus[contId + 1].err == 0 || m_PadStatus[contId + 1].err == -3) {
                
            }
            else if (m_PadStatus[contId + 1].err == -1) {
            
            }
        }
        if (m_tick != 0) {
            PADReset(m_tick);
        }
    }
}
