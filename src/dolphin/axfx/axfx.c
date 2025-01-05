#include "dolphin/axfx.h"
#include "dolphin/types.h"
#include "axfx/__axfx.h"

int __OSCurrHeap;

static void * __AXFXAllocFunction(u32 bytes) {

}

static void __AXFXFreeFunction(void * p) {

}

void * (* __AXFXAlloc)(u32);
void (* )

void AXFXSetHooks(AXFXAlloc, AXFXFree) {
    
}
