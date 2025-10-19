#include "game/parse.h"
#include "game/macros.h"
#include "game/memory.h"
#include "string.h"

void parse::ParseFile(char* fname, int path_index, u8* data, int unk)
{
    DEBUGASSERTLINE(105, m_pBuffer == NULL && !m_fileBuff.IsActive() && m_pData == NULL);
    if (data) {
        m_pData = data;
        unkE4 = unk;
    } else {
        strcpy(m_pFileName, fname);
        m_pBuffer = allocate_in_commonblock(0x2000);
        m_fileBuff.SetFileBuffer(m_pBuffer, 0x2000);
    }
}
