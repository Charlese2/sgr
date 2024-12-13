#include "dolphin/os/OSAlloc.h"
#include "dolphin/types.h"

extern BOOL lbl_8047558c;
volatile extern OSHeapHandle __OSCurrHeap;
char stringbuf [132];
extern void DebugError(char *, u32, char *);

u32 SomeMemoryFunction(int amount, char * file, int line){
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
    return (u32)ptrToAllocatedMemory;
}
