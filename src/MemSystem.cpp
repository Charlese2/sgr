#include "game/MemSystem.h"
#include "dolphin.h"
#include "dolphin/os.h"
#include "dolphin/os/OSAlloc.h"
#include "game/macros.h"
#include "macros.h"

MemSystem gMemSystem;

MemSystem::MemSystem() {
    m_heapHandle = -1;
}

MemSystem::~MemSystem() {
    OSDestroyHeap(m_heapHandle);
}

void MemSystem::CreateHeap() {
    void *arenaLo = OSGetArenaLo();
    void *arenaHi = OSGetArenaHi();
    arenaLo       = OSInitAlloc(arenaLo, arenaHi, 1);
    OSSetArenaLo(arenaLo);
    m_heapHandle = OSCreateHeap((void *)OSRoundUp32B(arenaLo), (void *)OSRoundDown32B(arenaHi));
    ASSERTLINE(62, m_heapHandle != -1);
    OSSetCurrentHeap(m_heapHandle);
    OSSetArenaLo(arenaHi);
}
