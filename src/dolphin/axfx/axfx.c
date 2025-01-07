#include "dolphin/axfx.h"
#include "dolphin/types.h"
#include "__axfx.h"

int __OSCurrHeap;

static void * __AXFXAllocFunction(u32 bytes) {

}

static void __AXFXFreeFunction(void * p) {

}

void * (* __AXFXAlloc)(u32);
void (* __AXFXFree)(void *);

void AXFXSetHooks(AXFXAlloc, AXFXFree) {
    
}
