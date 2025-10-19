#include "dolphin/types.h"

extern char string_buffer[512];

typedef void (*EffectCallback)(void);

class effects_mem_info {
  public:
    char **m_type;
    char *m_peg_id;
    u32 unk8;
    u32 unkC;
    u32 unk10;
    EffectCallback unk14;
    EffectCallback unk18;
    EffectCallback unk1C;
    EffectCallback unk20;
    EffectCallback unk24;
    EffectCallback unk28;
    int unk2C;
    int unk30;

    void initialize_info();
    void initialize() { initialize_info(); }
    bool verify() {
        if (!m_type) {
            return false;
        }
        if (!m_peg_id) {
            return false;
        }
        if (!unk8) {
            return false;
        }
        if (!unkC) {
            return false;
        }
        if (!unk10) {
            return false;
        }
        return true;
    };
};

extern effects_mem_info *Fx_mem_external_info;

class r_header {
  public:
    u16 m_reference_count;
    short unk2;
    short m_peg_id;
    int unk8;
    int unkC;
    int unk10;
    short unk14;
    bool unk16_01 : 1;
    bool unk16_02 : 1;
    bool m_is_loading : 1;
    int unk18;
    u8 *unk1C;
    char *m_filename;
};

int FreeEffect(r_header *rheader);
