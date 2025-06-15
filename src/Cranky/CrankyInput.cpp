#include "game/CrankyInput.h"

CrankyInput::CrankyInput() {
    CrankyInput::Initialize(false);
}

CrankyInput::~CrankyInput() {}

void CrankyInput::Initialize(bool static_init) {
    int i;
    int j;
    PADStatus *pPadStatus;

    m_pPadStatus = NULL;
    m_low        = 0.0f;
    m_high       = 0.0f;
    unk4         = 0.0f;
    unk8         = 0.0f;
    if (static_init) {
        PADInit();
        m_pPadStatus = (PADStatus *)PADRead(m_PadStatus);
        m_mask       = 0;
        for (i = 0; i < PAD_MAX_CONTROLLERS; i++) {
            pPadStatus = &m_PadStatus[i];
            if (pPadStatus->err == PAD_ERR_NONE || pPadStatus->err == PAD_ERR_TRANSFER) {
                unk18 |= GetControllerMask(i);
            } else if (pPadStatus->err == PAD_ERR_NO_CONTROLLER) {
                m_mask |= GetControllerMask(i);
            }
        }
        if (m_mask != 0) {
            PADReset(m_mask);
        }
    }
    for (j = 0; j < PAD_MAX_CONTROLLERS; j++) {
        InitializeControllerStatus(j);
    }
}

void CrankyInput::InitializeControllerStatus(int contId) {
    PADStatus *pPadStatus;
    button_status *pButtonStatus;

    ASSERTLINE(103, contId >= 0 && contId < PAD_MAX_CONTROLLERS);

    pPadStatus               = &m_PadStatus[contId];
    pPadStatus->button       = 0;
    pPadStatus->stickX       = 0;
    pPadStatus->stickY       = 0;
    pPadStatus->substickX    = 0;
    pPadStatus->substickY    = 0;
    pPadStatus->triggerLeft  = 0;
    pPadStatus->triggerRight = 0;
    pPadStatus->analogA      = 0;
    pPadStatus->analogB      = 0;
    pPadStatus->err          = false;
    pButtonStatus            = &m_buttonStatus[contId];
    pButtonStatus->unk0      = 0;
    pButtonStatus->buttons   = 0;
}
