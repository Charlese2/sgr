#include "game/memory.h"
#include "game/debug.h"
#include "dolphin/os.h"

Memory::MemoryStruct * pointerToMemoryStruct;

void * Memory::Allocate(size_t amount, const char* file, int line) {
    void * ptrToAllocatedMemory;
    char stringbuf[122];
    ptrToAllocatedMemory = OSAllocFromHeap(__OSCurrHeap, amount);
    if (ptrToAllocatedMemory == NULL) {
        sprintf(stringbuf, "Failed to allocate %d bytes\n", amount);
        DebugError("memory.cpp", 108, stringbuf);
        ptrToAllocatedMemory = NULL;
    }
    return ptrToAllocatedMemory;
}

void * Memory::operator new(size_t amount, const char* file, int line) {
    void * memAddressToUse;
    u32 offset;
    int remaining;
    u32 alignedRemaining;
    char * somePointer;
    if (pointerToMemoryStruct != NULL) {
        remaining = pointerToMemoryStruct->remaining - 1;
        offset = pointerToMemoryStruct->offset;

        alignedRemaining = offset + (~remaining & amount + remaining);
        
        if (alignedRemaining > pointerToMemoryStruct->unk4) {
            memAddressToUse = NULL;
        } else {
            somePointer = pointerToMemoryStruct->unk0;
            pointerToMemoryStruct->offset = alignedRemaining;
            return somePointer + offset;
        }
    } else {
        memAddressToUse = Allocate(amount, file, line);
    }
    return memAddressToUse;
}

void * Memory::operator new[](size_t amount, const char* file, int line) {
    void * memAddressToUse;
    u32 offset;
    int remaining;
    u32 alignedRemaining;
    char * somePointer;
    if (pointerToMemoryStruct != NULL) {
        remaining = pointerToMemoryStruct->remaining - 1;
        offset = pointerToMemoryStruct->offset;

        alignedRemaining = offset + (~remaining & amount + remaining);
        
        if (alignedRemaining > pointerToMemoryStruct->unk4) {
            memAddressToUse = NULL;
        } else {
            somePointer = pointerToMemoryStruct->unk0;
            pointerToMemoryStruct->offset = alignedRemaining;
            return somePointer + offset;
        }
    } else {
        memAddressToUse = Allocate(amount, file, line);
    }
    return memAddressToUse;
}

void Memory::operator delete(void * memoryAddress) throw () {
    OSFreeToHeap(__OSCurrHeap, memoryAddress);
}


void Memory::operator delete[](void * memoryAddress) throw () {
    OSFreeToHeap(__OSCurrHeap, memoryAddress);
}

void Memory::SetSomethingMemory(MemoryStruct * unk) {
    pointerToMemoryStruct = unk;
}

Memory::MemoryStruct * Memory::GetSomethingMemory(void) {
    return pointerToMemoryStruct;
}

void * Memory::AllocateInCommonBlock(u32 amount) {
    DebugError("memory.cpp", 449, "Common block is already locked.");
    DebugError( "memory.cpp", 452, "Not enough space in common block.\n");
}
