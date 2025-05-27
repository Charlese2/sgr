#include "game/CrankyFileManager.h"
#include "game/Cranky.h"
#include "dolphin/os.h"
#include "macros.h"

void CrankyFileBuffer::SetFileBuffer(u32* buf, int requested_size) {
    int size;

    m_pBuffer = (u32*)CrankyRoundUp32((u32)&buf);
    size = requested_size - (m_pBuffer - buf);
    m_BufferSize = CrankyAlign32(size);
    ASSERTLINE(107, (m_pBuffer + m_BufferSize) <= (buf + size));
}
