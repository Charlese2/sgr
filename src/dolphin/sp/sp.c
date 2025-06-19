#include "dolphin/sp.h"
#include "dolphin/ax.h"
#include "dolphin/os.h"
#include "macros.h"

void SPInitSoundTable(table* table, u32 aramBase, u32 zeroBase) {
    int i;
    SOUND_ENTRY* sound;
    ADPCM* adpcm;
    u32 aramBase4;
    u32 aramBase8;
    u32 aramBase16;
    u32 zeroBase8;
    u32 zeroBase4;
    u32 zeroBase16;

    ASSERTLINE(34, table);

    aramBase4 = aramBase * 2;
    zeroBase8 = zeroBase * 2 + 2;
    aramBase8 = aramBase;
    zeroBase4 = zeroBase;
    aramBase16 = aramBase >> 1;
    zeroBase16 = zeroBase >> 1;
    sound = table->sound;
    adpcm = (ADPCM*)&table->sound[table->entries];

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
                sound->loopAddr = zeroBase4;
                sound->loopEndAddr = 0;
                sound->endAddr = aramBase8 + sound->endAddr;
                sound->currentAddr = aramBase8 + sound->currentAddr;
                break;
            case 5:
                sound->loopAddr = aramBase8 + sound->loopAddr;
                sound->loopEndAddr = aramBase8 + sound->loopEndAddr;
                sound->endAddr = aramBase8 + sound->endAddr;
                sound->currentAddr = aramBase8 + sound->currentAddr;
                break;
        }
        sound++;
    }
}

SOUND_ENTRY * SPGetSoundEntry(table* table, u32 index) {
    ASSERTLINE(123, table);

    if (table->entries > index) {
        return &table->sound[index];
    }
    
    return 0;
}

void SPPrepareSound(SOUND_ENTRY* sound, AXVPB* axvpb, u32 sampleRate) {
    int old;
    u32 srcBits;
    u32 loopAddr;
    u32 endAddr;
    u32 currentAddr;
    u16* p;
    u16* p1;

    ASSERTLINE(140, sound);
    ASSERTLINE(141, axvpb);

    srcBits = sampleRate / 32000.0f * 65536.0f;

    switch (sound->type) {
        case 0:
            loopAddr = sound->loopAddr;
            endAddr = sound->endAddr;
            currentAddr = sound->currentAddr;
            p = (u16*)&axvpb->pb.addr;
            p1 = (u16*)sound->adpcm;
            old = OSDisableInterrupts();
            *p++ = 0;
            *p++ = 0;
            *p++ = (u16)(loopAddr >> 16);
            *p++ = (u16)(loopAddr);
            *p++ = (u16)(endAddr >> 16);
            *p++ = (u16)(endAddr);
            *p++ = (u16)(currentAddr >> 16);
            *p++ = (u16)(currentAddr);
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = (u16)(srcBits >> 16);
            *p++ = (u16)(srcBits);
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            axvpb->sync |= AX_SYNC_FLAG_COPYADDR | AX_SYNC_FLAG_COPYADPCM | AX_SYNC_FLAG_COPYSRC;
            OSRestoreInterrupts(old);
            break;
        case 1:
            loopAddr = sound->loopAddr;
            endAddr = sound->loopEndAddr;
            currentAddr = sound->currentAddr;
            p = (u16*)&axvpb->pb.addr;
            p1 = (u16*)sound->adpcm;
            old = OSDisableInterrupts();
            *p++ = 1;
            *p++ = 0;
            *p++ = (u16)(loopAddr >> 16);
            *p++ = (u16)(loopAddr);
            *p++ = (u16)(endAddr >> 16);
            *p++ = (u16)(endAddr);
            *p++ = (u16)(currentAddr >> 16);
            *p++ = (u16)(currentAddr);
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = (u16)(srcBits >> 16);
            *p++ = (u16)(srcBits);
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = *p1++;
            *p++ = *p1++;
            *p++ = *p1++;
            axvpb->sync |= AX_SYNC_FLAG_COPYADDR | AX_SYNC_FLAG_COPYADPCM | AX_SYNC_FLAG_COPYSRC | AX_SYNC_FLAG_COPYADPCMLOOP;
            OSRestoreInterrupts(old);
            break;
        case 2:
            loopAddr = sound->loopAddr;
            endAddr = sound->endAddr;
            currentAddr = sound->currentAddr;
            p = (u16*)&axvpb->pb.addr;
            p1 = (u16*)sound->adpcm;
            old = OSDisableInterrupts();
            *p++ = 0;
            *p++ = 10;
            *p++ = loopAddr >> 16;
            *p++ = loopAddr;
            *p++ = endAddr >> 16;
            *p++ = endAddr;
            *p++ = currentAddr >> 16;
            *p++ = currentAddr;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 2048;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = srcBits >> 16;
            *p++ = srcBits;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            axvpb->sync |= AX_SYNC_FLAG_COPYADDR | AX_SYNC_FLAG_COPYADPCM | AX_SYNC_FLAG_COPYSRC;
            OSRestoreInterrupts(old);
            break;
        case 3:
            loopAddr = sound->loopAddr;
            endAddr = sound->loopEndAddr;
            currentAddr = sound->currentAddr;
            p = (u16*)&axvpb->pb.addr;
            p1 = (u16*)sound->adpcm;
            old = OSDisableInterrupts();
            *p++ = 1;
            *p++ = 10;
            *p++ = loopAddr >> 16;
            *p++ = loopAddr;
            *p++ = endAddr >> 16;
            *p++ = endAddr;
            *p++ = currentAddr >> 16;
            *p++ = currentAddr;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0x800;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = srcBits >> 16;
            *p++ = srcBits;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            axvpb->sync |= AX_SYNC_FLAG_COPYADDR | AX_SYNC_FLAG_COPYADPCM | AX_SYNC_FLAG_COPYSRC;
            OSRestoreInterrupts(old);
            break;
        case 4:
            loopAddr = sound->loopAddr;
            endAddr = sound->endAddr;
            currentAddr = sound->currentAddr;
            p = (u16*)&axvpb->pb.addr;
            p1 = (u16*)sound->adpcm;
            old = OSDisableInterrupts();
            *p++ = 0;
            *p++ = 25;
            *p++ = loopAddr >> 16;
            *p++ = loopAddr;
            *p++ = endAddr >> 16;
            *p++ = endAddr;
            *p++ = currentAddr >> 16;
            *p++ = currentAddr;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0x100;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = srcBits >> 16;
            *p++ = srcBits;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            axvpb->sync |= AX_SYNC_FLAG_COPYADDR | AX_SYNC_FLAG_COPYADPCM | AX_SYNC_FLAG_COPYSRC;
            OSRestoreInterrupts(old);
            break;
        case 5:
            loopAddr = sound->loopAddr;
            endAddr = sound->loopEndAddr;
            currentAddr = sound->currentAddr;
            p = (u16*)&axvpb->pb.addr;
            p1 = (u16*)sound->adpcm;
            old = OSDisableInterrupts();
            *p++ = 1;
            *p++ = 25;
            *p++ = loopAddr >> 16;
            *p++ = loopAddr;
            *p++ = endAddr >> 16;
            *p++ = endAddr;
            *p++ = currentAddr >> 16;
            *p++ = currentAddr;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 256;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = srcBits >> 16;
            *p++ = srcBits;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            *p++ = 0;
            axvpb->sync |= AX_SYNC_FLAG_COPYADDR | AX_SYNC_FLAG_COPYADPCM | AX_SYNC_FLAG_COPYSRC;
            OSRestoreInterrupts(old);
            break;
    }
}

void SPPrepareEnd(SOUND_ENTRY *sound, AXVPB *axvpb) {
    int old;
}
