#include "game/memory.h"
#include "game/debug.h"

void * SomeMemoryFunction(u32 amount, const char *, int line){
    void * ptrToAllocatedMemory;
    OSHeapHandle heap = __OSCurrHeap;
    if (lbl_8047558c) {
        
    } else {
        ptrToAllocatedMemory = OSAllocFromHeap(heap, amount);
        if (ptrToAllocatedMemory == NULL){
            sprintf(stringbuf, "Failed to allocate %d bytes\n", amount);
            DebugError(__FILE__, 108, stringbuf);
            ptrToAllocatedMemory = NULL;
        }
    }
    return (void *)ptrToAllocatedMemory;
}
