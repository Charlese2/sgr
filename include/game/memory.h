#include "dolphin/types.h"
#include "dolphin/os/OSAlloc.h"

extern BOOL lbl_8047558c;
volatile extern OSHeapHandle __OSCurrHeap;
char stringbuf [132];

template<typename T>
void * SomeMemoryFunction(int amount, T file, int line);
