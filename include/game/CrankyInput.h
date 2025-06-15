#include "dolphin/os.h"
#include "dolphin/pad.h"
#include "global.h"

typedef struct button_status {
    int unk0;
    u16 buttons;
} controller_status;

class CrankyInput {
  public:
    virtual ~CrankyInput();
    CrankyInput();
    void Initialize(bool static_init);
    void InitializeControllerStatus(int contId);
    int GetControllerMask(int contId) {
      return 0x10000000 << (3 - contId);
    };

    float unk4;
    float unk8;
    float m_low;
    float m_high;
    PADStatus *m_pPadStatus;
    int unk18;
    u32 m_mask;
    OSTick m_tick;
    PADStatus m_PadStatus[PAD_MAX_CONTROLLERS];
    button_status m_buttonStatus[PAD_MAX_CONTROLLERS];
};

STATIC_ASSERT(sizeof(CrankyInput) == 0x74);
