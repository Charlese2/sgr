#include "dolphin/os.h"
#include "dolphin/pad.h"
#include "global.h"

class CrankyInput {
  public:
    virtual ~CrankyInput();
    CrankyInput();
    void Initialize(bool static_init);

    float unk4;
    float unk8;
    float low;
    float high;
    PADStatus *m_pPadStatus;
    int unk18;
    OSTick m_tick;
    u32 m_mask;
    PADStatus m_PadStatus[PAD_MAX_CONTROLLERS];
    char unk[4];
};

STATIC_ASSERT(sizeof(CrankyInput) == 0x58);
