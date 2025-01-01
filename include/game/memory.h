#include "dolphin/os/OSException.h"
#include "dolphin/types.h"
extern "C" {
    #include "dolphin/os/OSAlloc.h"
}


extern BOOL lbl_8047558c;
volatile extern OSHeapHandle __OSCurrHeap;
char stringbuf [132];

void operator delete(void * address) throw();
void operator delete[](void * memoryAddress) throw ();

void* operator new(size_t amount, char* file, int line);
