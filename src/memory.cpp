#include "game/memory.h"
#include "dolphin/os/OSAlloc.h"
#include "dolphin/os/OSException.h"
#include "game/debug.h"

void operator delete[](void* memoryAddress) throw () {
    OSFreeToHeap(__OSCurrHeap, memoryAddress);
}

void operator delete(void* memoryAddress) throw () {
    OSFreeToHeap(__OSCurrHeap, memoryAddress);
}

void* operator new[](size_t amount, const char*, int line) {
    void* ptrToAllocatedMemory;
    OSHeapHandle heap = __OSCurrHeap;
    try {
        ptrToAllocatedMemory = OSAllocFromHeap(heap, amount);
    }
    catch (__OSException e) {
        sprintf(stringbuf, "Failed to allocate %d bytes\n", amount);
        DebugError(__FILE__, 108, stringbuf);
        ptrToAllocatedMemory = NULL;
    }
    return ptrToAllocatedMemory;
}
