#include "game/memory.h"
#include "game/debug.h"
#include "dolphin/os.h"

#define RoundUpToStride(value, stride) (~stride & value + stride)

Memory * globalMemoryStruct;
u32 somethingCommonBlock; 
u32 commonBlockSpaceUsed; 
char CommonBlock[COMMON_BLOCK_SIZE];
const char empty[0xfa0] = "";
extern s32 lbl_804740F0[2];

void * Allocate(size_t amount, const char* file, int line) {
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

void * operator new(size_t amount, const char* file, int line) {
    void * memAddressToUse;
    u32 offset;
    int stride;
    u32 newOffset;
    char * destination;
    if (isFinished()) {
        stride = globalMemoryStruct->stride - 1;
        offset = globalMemoryStruct->offset;

        newOffset = offset + RoundUpToStride(amount, stride);
        
        if (newOffset > globalMemoryStruct->size) {
            memAddressToUse = NULL;
        } else {
            destination = globalMemoryStruct->destination;
            globalMemoryStruct->offset = newOffset;
            return destination + offset;
        }
    } else {
        memAddressToUse = Allocate(amount, file, line);
    }
    return memAddressToUse;
}

void * operator new[](size_t amount, const char* file, int line) {
    void * memAddressToUse;
    u32 offset;
    int stride;
    u32 newOffset;
    char * somePointer;
    if (globalMemoryStruct != NULL) {
        stride = globalMemoryStruct->stride - 1;
        offset = globalMemoryStruct->offset;

        newOffset = offset + RoundUpToStride(amount, stride);
        
        if (newOffset > globalMemoryStruct->size) {
            memAddressToUse = NULL;
        } else {
            somePointer = globalMemoryStruct->destination;
            globalMemoryStruct->offset = newOffset;
            return somePointer + offset;
        }
    } else {
        memAddressToUse = Allocate(amount, file, line);
    }
    return memAddressToUse;
}

void operator delete(void * memoryAddress) throw () {
    OSFreeToHeap(__OSCurrHeap, memoryAddress);
}


void operator delete[](void * memoryAddress) throw () {
    OSFreeToHeap(__OSCurrHeap, memoryAddress);
}

void copy(Memory * memoryStruct,  char * destination, u32 size, char * name, u8 stride) {
    strcpy(memoryStruct->name, name);
    memoryStruct->destination = destination;
    memoryStruct->size = size;
    memoryStruct->offset = 0;
    memoryStruct->stride = stride;
}

void * getOffset(Memory * memoryStruct, u32 unk) {
    u32 offset;
    int stride;
    u32 newOffset;
    char * destination;
    offset = memoryStruct->offset;
    stride = memoryStruct->stride - 1;

    newOffset = offset + (~stride & unk + stride);
    if (newOffset > memoryStruct->size) {
        return 0;
    }

    destination = memoryStruct->destination;

    memoryStruct->offset = newOffset;
    return (destination + offset);
}

void SetSomethingMemory(Memory * unk) {
    globalMemoryStruct = unk;
}

Memory * GetSomethingMemory(void) {
    return globalMemoryStruct;
}

BOOL isFinished() {
    return globalMemoryStruct != 0;
}

u32 getCommonBlockSpaceFree() {
    return COMMON_BLOCK_SIZE - commonBlockSpaceUsed;
}

void * AllocateInCommonBlock(u32 amount) {
    u32 alignedAmount;
    char * allocatedAddress;

    alignedAmount = OSRoundUp32B(amount);
    if (lbl_804740F0[somethingCommonBlock] != -1) {
        DebugError("memory.cpp", 449, "Common block is already locked.");
    }

    if (alignedAmount > getCommonBlockSpaceFree()) {
        DebugError( "memory.cpp", 452, "Not enough space in common block.\n");
    }
    
    allocatedAddress = CommonBlock + commonBlockSpaceUsed;
    lbl_804740F0[somethingCommonBlock] = alignedAmount;
    commonBlockSpaceUsed += alignedAmount;
    somethingCommonBlock = somethingCommonBlock + 1;
    return allocatedAddress;
}

void setSomethingCommonBlock(u32 amount) {
    int test;
    if (amount != 0) {
        somethingCommonBlock--;
        test = lbl_804740F0[somethingCommonBlock];
        lbl_804740F0[somethingCommonBlock] = -1;
        commonBlockSpaceUsed = commonBlockSpaceUsed - test;
    }
}
