#include "game/memory.h"
#include "game/debug.h"
#include "dolphin/os.h"

#define RoundUpToAlignment(value, alignment) (~alignment & value + alignment)

Memory * Pool;
u32 somethingCommonBlock; 
u32 commonBlockSpaceUsed; 
char CommonBlock[COMMON_BLOCK_SIZE];
const char empty[4000] = "";
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

void * operator new(size_t amount, const char * file, int line) {
    void * memAddressToUse;
    int alignment;
    u32 offset;
    u32 newOffset;
    char * destination;
    if (isFinished()) {
        alignment = Pool->alignment - 1;
        offset = Pool->offset;

        newOffset = offset + RoundUpToAlignment(amount, alignment);
        
        if (newOffset > Pool->size) {
            memAddressToUse = NULL;
        } else {
            destination = Pool->destination;
            Pool->offset = newOffset;
            return destination + offset;
        }
    } else {
        memAddressToUse = Allocate(amount, file, line);
    }
    return memAddressToUse;
}

void * operator new[](size_t amount, const char * file, int line) {
    void * memAddressToUse;
    int alignment;
    u32 offset;
    u32 newOffset;
    char * destination;
    if (isFinished()) {
        alignment = Pool->alignment - 1;
        offset = Pool->offset;

        newOffset = offset + RoundUpToAlignment(amount, alignment);
        
        if (newOffset > Pool->size) {
            memAddressToUse = NULL;
        } else {
            destination = Pool->destination;
            Pool->offset = newOffset;
            return destination + offset;
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

void copy(Memory * mem_pool,  char * destination, u32 size, char * name, u8 alignment) {
    strcpy(mem_pool->pool_name, name);
    mem_pool->destination = destination;
    mem_pool->size = size;
    mem_pool->offset = 0;
    mem_pool->alignment = alignment;
}

void * getOffset(Memory * memoryStruct, u32 amount) {
    int alignment;
    u32 offset;
    u32 newOffset;
    char * destination;
    offset = memoryStruct->offset;
    alignment = memoryStruct->alignment - 1;

    newOffset = offset + (~alignment & amount + alignment);
    if (newOffset > memoryStruct->size) {
        return 0;
    }

    destination = memoryStruct->destination;

    memoryStruct->offset = newOffset;
    return destination + offset;
}

void SetCurrentMempool(Memory * pool) {
    Pool = pool;
}

Memory * GetCurrentMempool(void) {
    return Pool;
}

BOOL isFinished() {
    return Pool != 0;
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
    int unk;
    if (amount != 0) {
        somethingCommonBlock--;
        unk = lbl_804740F0[somethingCommonBlock];
        lbl_804740F0[somethingCommonBlock] = -1;
        commonBlockSpaceUsed = commonBlockSpaceUsed - unk;
    }
}
