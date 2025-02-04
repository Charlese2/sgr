#include "dolphin/sp.h"
#include "dolphin/os.h"
#include "macros.h"

void SPInitSoundTable(table* table, u32 aramBase, u32 zeroBase) {
    int i;
    SOUND_ENTRY* sound;
    ADPCM* adpcm;
    u32 aramBase4;
    u32 aramBase8;
    u32 aramBase16;
    u32 zeroBase4;
    u32 zeroBase8;
    u32 zeroBase16;

    ASSERTLINE(34, table);

    aramBase4 = aramBase * 2;
    aramBase8 = aramBase * 2 + 2;
    aramBase16 = aramBase >> 1;
    zeroBase4 = zeroBase * 2;
    zeroBase8 = zeroBase * 2 + 2;
    zeroBase16 = zeroBase >> 1;
    sound = table->sound;
    adpcm = table->sound->adpcm;

    for (i = 0; i < table->entries; i++) {
        switch (sound->type) {
            case 0:
                sound->loopAddr = zeroBase8;
                sound->loopEndAddr = 0;
                sound->endAddr = aramBase4 + sound->endAddr;
                sound->currentAddr = aramBase4 + sound->currentAddr;
                sound->adpcm = adpcm;
                adpcm++;
                break;
            case 1:
                sound->loopAddr = aramBase4 + sound->loopAddr;
                sound->loopEndAddr = aramBase4 + sound->loopEndAddr;
                sound->endAddr = aramBase4 + sound->endAddr;
                sound->currentAddr = aramBase4 + sound->currentAddr;
                sound->adpcm = adpcm;
                adpcm++;
                break;
            case 2:
                sound->loopAddr = zeroBase16;
                sound->loopEndAddr = 0;
                sound->endAddr = aramBase16 + sound->endAddr;
                sound->currentAddr = aramBase16 + sound->currentAddr;
                break;
            case 3:
                sound->loopAddr = aramBase16 + sound->loopAddr;
                sound->loopEndAddr = aramBase16 + sound->loopEndAddr;
                sound->endAddr = aramBase16 + sound->endAddr;
                sound->currentAddr = aramBase16 + sound->currentAddr;
                break;
            case 4:
                sound->loopAddr = zeroBase;
                sound->loopEndAddr = 0;
                sound->endAddr = aramBase + sound->endAddr;
                sound->currentAddr = aramBase + sound->currentAddr;
                break;
            case 5:
                sound->loopAddr = aramBase + sound->loopAddr;
                sound->loopEndAddr = aramBase + sound->loopEndAddr;
                sound->endAddr = aramBase + sound->endAddr;
                sound->currentAddr = aramBase + sound->currentAddr;
                break;
        }
        sound++;
    }
}

SOUND_ENTRY * SPGetSoundEntry(table* table, u32 index) {
    ASSERTLINE(123, table);

    if (table->entries > index) {
        return table->sound + index;
    }
    
    return 0;
}

void SPPrepareSound(SOUND_ENTRY* sound, AXVPB * axvpb, u32 sampleRate) {
    int old;
    u32 srcBits, loopAddr, endAddr, currentAddr;
    u16* p, p1;

    ASSERTLINE(140, sound);
    ASSERTLINE(141, axvpb);

    srcBits = sampleRate / 32000.0f * 65536.0f;

    switch (sound->type) {
        case 0:
            loopAddr = sound->loopAddr;
            endAddr = sound->endAddr;
            currentAddr = sound->currentAddr;
            p = axvpb->updateWrite;
            old = OSDisableInterrupts();
            axvpb->pb.addr.loopFlag = 0;
            axvpb->pb.addr.format = 0;
            axvpb->pb.addr.loopAddressHi = loopAddr >> 16;
            axvpb->pb.addr.loopAddressLo = loopAddr;
            axvpb->pb.addr.endAddressHi = endAddr >> 16;
            axvpb->pb.addr.endAddressLo = endAddr;
            axvpb->pb.addr.currentAddressHi = currentAddr >> 16;
            axvpb->pb.addr.currentAddressLo = currentAddr;
            axvpb->pb.adpcm.a[0][0] = (sound->adpcm->adpcm).a[0][0];
            axvpb->pb.adpcm.a[0][1] = (sound->adpcm->adpcm).a[0][1];
            axvpb->pb.adpcm.a[1][0] = (sound->adpcm->adpcm).a[1][0];
            axvpb->pb.adpcm.a[1][1] = (sound->adpcm->adpcm).a[1][1];
            axvpb->pb.adpcm.a[2][0] = (sound->adpcm->adpcm).a[2][0];
            axvpb->pb.adpcm.a[2][1] = (sound->adpcm->adpcm).a[2][1];
            axvpb->pb.adpcm.a[3][0] = (sound->adpcm->adpcm).a[3][0];
            axvpb->pb.adpcm.a[3][1] = (sound->adpcm->adpcm).a[3][1];
            axvpb->pb.adpcm.a[4][0] = (sound->adpcm->adpcm).a[4][0];
            axvpb->pb.adpcm.a[4][1] = (sound->adpcm->adpcm).a[4][1];
            axvpb->pb.adpcm.a[5][0] = (sound->adpcm->adpcm).a[5][0];
            axvpb->pb.adpcm.a[5][1] = (sound->adpcm->adpcm).a[5][1];
            axvpb->pb.adpcm.a[6][0] = (sound->adpcm->adpcm).a[6][0];
            axvpb->pb.adpcm.a[6][1] = (sound->adpcm->adpcm).a[6][1];
            axvpb->pb.adpcm.a[7][0] = (sound->adpcm->adpcm).a[7][0];
            axvpb->pb.adpcm.a[7][1] = (sound->adpcm->adpcm).a[7][1];
            axvpb->pb.adpcm.gain = (sound->adpcm->adpcm).gain;
            axvpb->pb.adpcm.pred_scale = (sound->adpcm->adpcm).pred_scale;
            axvpb->pb.adpcm.yn1 = (sound->adpcm->adpcm).yn1;
            axvpb->pb.adpcm.yn2 = (sound->adpcm->adpcm).yn2;
            axvpb->pb.src.ratioHi = srcBits >> 16;
            axvpb->pb.src.ratioLo = srcBits;
            axvpb->pb.src.currentAddressFrac = 0;
            axvpb->pb.src.last_samples[0] = 0;
            axvpb->pb.src.last_samples[1] = 0;
            axvpb->pb.src.last_samples[2] = 0;
            axvpb->pb.src.last_samples[3] = 0;
            axvpb->sync = axvpb->sync | AX_SYNC_FLAG_COPYADDR | AX_SYNC_FLAG_COPYADPCM | AX_SYNC_FLAG_COPYSRC;
            OSRestoreInterrupts(old);
            break;
        case 1:
            loopAddr = sound->loopAddr;
            endAddr = sound->loopEndAddr;
            currentAddr = sound->currentAddr;
            old = OSDisableInterrupts();
            axvpb->pb.addr.loopFlag = 1;
            axvpb->pb.addr.format = 0;
            axvpb->pb.addr.loopAddressHi = loopAddr >> 16;
            axvpb->pb.addr.loopAddressLo = loopAddr;
            axvpb->pb.addr.endAddressHi = endAddr >> 16;
            axvpb->pb.addr.endAddressLo = endAddr;
            axvpb->pb.addr.currentAddressHi = currentAddr >> 16;
            axvpb->pb.addr.currentAddressLo = currentAddr;
            axvpb->pb.adpcm.a[0][0] = (sound->adpcm->adpcm).a[0][0];
            axvpb->pb.adpcm.a[0][1] = (sound->adpcm->adpcm).a[0][1];
            axvpb->pb.adpcm.a[1][0] = (sound->adpcm->adpcm).a[1][0];
            axvpb->pb.adpcm.a[1][1] = (sound->adpcm->adpcm).a[1][1];
            axvpb->pb.adpcm.a[2][0] = (sound->adpcm->adpcm).a[2][0];
            axvpb->pb.adpcm.a[2][1] = (sound->adpcm->adpcm).a[2][1];
            axvpb->pb.adpcm.a[3][0] = (sound->adpcm->adpcm).a[3][0];
            axvpb->pb.adpcm.a[3][1] = (sound->adpcm->adpcm).a[3][1];
            axvpb->pb.adpcm.a[4][0] = (sound->adpcm->adpcm).a[4][0];
            axvpb->pb.adpcm.a[4][1] = (sound->adpcm->adpcm).a[4][1];
            axvpb->pb.adpcm.a[5][0] = (sound->adpcm->adpcm).a[5][0];
            axvpb->pb.adpcm.a[5][1] = (sound->adpcm->adpcm).a[5][1];
            axvpb->pb.adpcm.a[6][0] = (sound->adpcm->adpcm).a[6][0];
            axvpb->pb.adpcm.a[6][1] = (sound->adpcm->adpcm).a[6][1];
            axvpb->pb.adpcm.a[7][0] = (sound->adpcm->adpcm).a[7][0];
            axvpb->pb.adpcm.a[7][1] = (sound->adpcm->adpcm).a[7][1];
            axvpb->pb.adpcm.gain = (sound->adpcm->adpcm).gain;
            axvpb->pb.adpcm.pred_scale = (sound->adpcm->adpcm).pred_scale;
            axvpb->pb.adpcm.yn1 = (sound->adpcm->adpcm).yn1;
            axvpb->pb.adpcm.yn2 = (sound->adpcm->adpcm).yn2;
            axvpb->pb.src.ratioHi = srcBits >> 16;
            axvpb->pb.src.ratioLo = srcBits;
            axvpb->pb.src.currentAddressFrac = 0;
            axvpb->pb.src.last_samples[0] = 0;
            axvpb->pb.src.last_samples[1] = 0;
            axvpb->pb.src.last_samples[2] = 0;
            axvpb->pb.src.last_samples[3] = 0;
            axvpb->pb.adpcmLoop.loop_pred_scale = (sound->adpcm->adpcmloop).loop_pred_scale;
            axvpb->pb.adpcmLoop.loop_yn1 = (sound->adpcm->adpcmloop).loop_yn1;
            axvpb->pb.adpcmLoop.loop_yn2 = (sound->adpcm->adpcmloop).loop_yn2;
            axvpb->sync = axvpb->sync | AX_SYNC_FLAG_COPYADDR | AX_SYNC_FLAG_COPYADPCM | AX_SYNC_FLAG_COPYSRC | AX_SYNC_FLAG_COPYADPCMLOOP;
            OSRestoreInterrupts(old);
            break;
        case 2:
            loopAddr = sound->loopAddr;
            endAddr = sound->loopEndAddr;
            currentAddr = sound->currentAddr;
            old = OSDisableInterrupts();
            axvpb->pb.addr.loopFlag = 0;
            axvpb->pb.addr.format = 10;
            axvpb->pb.addr.loopAddressHi = loopAddr >> 16;
            axvpb->pb.addr.loopAddressLo = loopAddr;
            axvpb->pb.addr.endAddressHi = endAddr >> 16;
            axvpb->pb.addr.endAddressLo = endAddr;
            axvpb->pb.addr.currentAddressHi = currentAddr >> 16;
            axvpb->pb.addr.currentAddressLo = currentAddr;
            axvpb->pb.adpcm.a[0][0] = 0;
            axvpb->pb.adpcm.a[0][1] = 0;
            axvpb->pb.adpcm.a[1][0] = 0;
            axvpb->pb.adpcm.a[1][1] = 0;
            axvpb->pb.adpcm.a[2][0] = 0;
            axvpb->pb.adpcm.a[2][1] = 0;
            axvpb->pb.adpcm.a[3][0] = 0;
            axvpb->pb.adpcm.a[3][1] = 0;
            axvpb->pb.adpcm.a[4][0] = 0;
            axvpb->pb.adpcm.a[4][1] = 0;
            axvpb->pb.adpcm.a[5][0] = 0;
            axvpb->pb.adpcm.a[5][1] = 0;
            axvpb->pb.adpcm.a[6][0] = 0;
            axvpb->pb.adpcm.a[6][1] = 0;
            axvpb->pb.adpcm.a[7][0] = 0;
            axvpb->pb.adpcm.a[7][1] = 0;
            axvpb->pb.adpcm.gain = 2048;
            axvpb->pb.adpcm.pred_scale = 0;
            axvpb->pb.adpcm.yn1 = 0;
            axvpb->pb.adpcm.yn2 = 0;
            axvpb->pb.src.ratioHi = srcBits >> 16;
            axvpb->pb.src.ratioLo = srcBits;
            axvpb->pb.src.currentAddressFrac = 0;
            axvpb->pb.src.last_samples[0] = 0;
            axvpb->pb.src.last_samples[1] = 0;
            axvpb->pb.src.last_samples[2] = 0;
            axvpb->pb.src.last_samples[3] = 0;
            axvpb->sync = axvpb->sync | AX_SYNC_FLAG_COPYADDR | AX_SYNC_FLAG_COPYADPCM | AX_SYNC_FLAG_COPYSRC;
            OSRestoreInterrupts(old);
            break;
        case 3:
            loopAddr = sound->loopAddr;
            endAddr = sound->loopEndAddr;
            currentAddr = sound->currentAddr;
            old = OSDisableInterrupts();
            axvpb->pb.addr.loopFlag = 1;
            axvpb->pb.addr.format = 10;
            axvpb->pb.addr.loopAddressHi = loopAddr >> 16;
            axvpb->pb.addr.loopAddressLo = loopAddr;
            axvpb->pb.addr.endAddressHi = endAddr >> 16;
            axvpb->pb.addr.endAddressLo = endAddr;
            axvpb->pb.addr.currentAddressHi = currentAddr >> 16;
            axvpb->pb.addr.currentAddressLo = currentAddr;
            axvpb->pb.adpcm.a[0][0] = 0;
            axvpb->pb.adpcm.a[0][1] = 0;
            axvpb->pb.adpcm.a[1][0] = 0;
            axvpb->pb.adpcm.a[1][1] = 0;
            axvpb->pb.adpcm.a[2][0] = 0;
            axvpb->pb.adpcm.a[2][1] = 0;
            axvpb->pb.adpcm.a[3][0] = 0;
            axvpb->pb.adpcm.a[3][1] = 0;
            axvpb->pb.adpcm.a[4][0] = 0;
            axvpb->pb.adpcm.a[4][1] = 0;
            axvpb->pb.adpcm.a[5][0] = 0;
            axvpb->pb.adpcm.a[5][1] = 0;
            axvpb->pb.adpcm.a[6][0] = 0;
            axvpb->pb.adpcm.a[6][1] = 0;
            axvpb->pb.adpcm.a[7][0] = 0;
            axvpb->pb.adpcm.a[7][1] = 0;
            axvpb->pb.adpcm.gain = 0x800;
            axvpb->pb.adpcm.pred_scale = 0;
            axvpb->pb.adpcm.yn1 = 0;
            axvpb->pb.adpcm.yn2 = 0;
            axvpb->pb.src.ratioHi = srcBits >> 16;
            axvpb->pb.src.ratioLo = srcBits;
            axvpb->pb.src.currentAddressFrac = 0;
            axvpb->pb.src.last_samples[0] = 0;
            axvpb->pb.src.last_samples[1] = 0;
            axvpb->pb.src.last_samples[2] = 0;
            axvpb->pb.src.last_samples[3] = 0;
            axvpb->sync = axvpb->sync | 0x61000;
            OSRestoreInterrupts(old);
            break;
        case 4:
            loopAddr = sound->loopAddr;
            endAddr = sound->loopEndAddr;
            currentAddr = sound->currentAddr;
            old = OSDisableInterrupts();
            axvpb->pb.addr.loopFlag = 0;
            axvpb->pb.addr.format = 25;
            axvpb->pb.addr.loopAddressHi = loopAddr >> 16;
            axvpb->pb.addr.loopAddressLo = loopAddr;
            axvpb->pb.addr.endAddressHi = endAddr >> 16;
            axvpb->pb.addr.endAddressLo = endAddr;
            axvpb->pb.addr.currentAddressHi = currentAddr >> 16;
            axvpb->pb.addr.currentAddressLo = currentAddr;
            axvpb->pb.adpcm.a[0][0] = 0;
            axvpb->pb.adpcm.a[0][1] = 0;
            axvpb->pb.adpcm.a[1][0] = 0;
            axvpb->pb.adpcm.a[1][1] = 0;
            axvpb->pb.adpcm.a[2][0] = 0;
            axvpb->pb.adpcm.a[2][1] = 0;
            axvpb->pb.adpcm.a[3][0] = 0;
            axvpb->pb.adpcm.a[3][1] = 0;
            axvpb->pb.adpcm.a[4][0] = 0;
            axvpb->pb.adpcm.a[4][1] = 0;
            axvpb->pb.adpcm.a[5][0] = 0;
            axvpb->pb.adpcm.a[5][1] = 0;
            axvpb->pb.adpcm.a[6][0] = 0;
            axvpb->pb.adpcm.a[6][1] = 0;
            axvpb->pb.adpcm.a[7][0] = 0;
            axvpb->pb.adpcm.a[7][1] = 0;
            axvpb->pb.adpcm.gain = 0x100;
            axvpb->pb.adpcm.pred_scale = 0;
            axvpb->pb.adpcm.yn1 = 0;
            axvpb->pb.adpcm.yn2 = 0;
            axvpb->pb.src.ratioHi = srcBits >> 16;
            axvpb->pb.src.ratioLo = srcBits;
            axvpb->pb.src.currentAddressFrac = 0;
            axvpb->pb.src.last_samples[0] = 0;
            axvpb->pb.src.last_samples[1] = 0;
            axvpb->pb.src.last_samples[2] = 0;
            axvpb->pb.src.last_samples[3] = 0;
            axvpb->sync = axvpb->sync | AX_SYNC_FLAG_COPYADDR | AX_SYNC_FLAG_COPYADPCM | AX_SYNC_FLAG_COPYSRC;
            OSRestoreInterrupts(old);
            break;
        case 5:
            loopAddr = sound->loopAddr;
            endAddr = sound->loopEndAddr;
            currentAddr = sound->currentAddr;
            old = OSDisableInterrupts();
            axvpb->pb.addr.loopFlag = 1;
            axvpb->pb.addr.format = 25;
            axvpb->pb.addr.loopAddressHi = loopAddr >> 16;
            axvpb->pb.addr.loopAddressLo = loopAddr;
            axvpb->pb.addr.endAddressHi = endAddr >> 16;
            axvpb->pb.addr.endAddressLo = endAddr;
            axvpb->pb.addr.currentAddressHi = currentAddr >> 16;
            axvpb->pb.addr.currentAddressLo = currentAddr;
            axvpb->pb.adpcm.a[0][0] = 0;
            axvpb->pb.adpcm.a[0][1] = 0;
            axvpb->pb.adpcm.a[1][0] = 0;
            axvpb->pb.adpcm.a[1][1] = 0;
            axvpb->pb.adpcm.a[2][0] = 0;
            axvpb->pb.adpcm.a[2][1] = 0;
            axvpb->pb.adpcm.a[3][0] = 0;
            axvpb->pb.adpcm.a[3][1] = 0;
            axvpb->pb.adpcm.a[4][0] = 0;
            axvpb->pb.adpcm.a[4][1] = 0;
            axvpb->pb.adpcm.a[5][0] = 0;
            axvpb->pb.adpcm.a[5][1] = 0;
            axvpb->pb.adpcm.a[6][0] = 0;
            axvpb->pb.adpcm.a[6][1] = 0;
            axvpb->pb.adpcm.a[7][0] = 0;
            axvpb->pb.adpcm.a[7][1] = 0;
            axvpb->pb.adpcm.gain = 256;
            axvpb->pb.adpcm.pred_scale = 0;
            axvpb->pb.adpcm.yn1 = 0;
            axvpb->pb.adpcm.yn2 = 0;
            axvpb->pb.src.ratioHi = srcBits >> 16;
            axvpb->pb.src.ratioLo = srcBits;
            axvpb->pb.src.currentAddressFrac = 0;
            axvpb->pb.src.last_samples[0] = 0;
            axvpb->pb.src.last_samples[1] = 0;
            axvpb->pb.src.last_samples[2] = 0;
            axvpb->pb.src.last_samples[3] = 0;
            axvpb->sync = axvpb->sync | AX_SYNC_FLAG_COPYADDR | AX_SYNC_FLAG_COPYADPCM | AX_SYNC_FLAG_COPYSRC;
            OSRestoreInterrupts(old);
            break;
    }
}

void SPPrepareEnd(SOUND_ENTRY *sound, AXVPB *axvpb) {
    int old;
}
