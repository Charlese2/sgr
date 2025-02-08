#include "game/memory.h"
#include "game/debug.h"
#include "dolphin/os.h"
#include "game/macros.h"

Memory * Pool;
u32 Common_block_index; 
u32 Bytes_used; 
char COMMON_BLOCK[COMMON_BLOCK_SIZE];
const char empty[4000] = "";
extern s32 Common_block_allocation_amount[2];
volatile extern OSHeapHandle __OSCurrHeap;
bool gHeapAlloc;

char* Allocate(size_t size) {
    char* t;
    char stringbuf[128];
    if (Pool) {
        t = AllocateInPool(Pool, size);
    } else {
        DEBUGASSERTLINE(100, gHeapAlloc == true);
        DEBUGASSERTLINE(102, size > 0);
        
        t = (char*)OSAllocFromHeap(__OSCurrHeap, size);
        if (t == 0) {
            sprintf(stringbuf, "Failed to allocate %d bytes\n", size);
            DebugError("memory.cpp", 108, stringbuf);
            t = 0;
        } else {
            DEBUGASSERTLINE(114, (t & 15) == 0);
        }
    }
    return t;
}

char* AllocateArray(size_t size, const char * file, int line) {
    return Allocate(size);
}

void Free(void * p) throw () {
    OSFreeToHeap(__OSCurrHeap, p);
}


void FreeArray(void * p) throw () {
    Free(p);
}

void Copy(Memory* mem_pool,  char* destination, u32 size, char* name, u8 alignment) {
    strcpy(mem_pool->pool_name, name);
    mem_pool->destination = destination;
    mem_pool->size = size;
    mem_pool->offset = 0;
    mem_pool->alignment = alignment;
}

char* AllocateInPool(Memory* pool, u32 size) {
    u32 offset;
    u32 poolSpaceLeft;
    u32 alignedSize;
    char* destination;

    alignedSize = ~(pool->alignment -1) & size + (pool->alignment - 1);
    if (pool->offset + alignedSize > pool->size) {
        return 0;
    }


    destination = pool->destination;
    offset = pool->offset;
    pool->offset = pool->offset + alignedSize;
    return destination + offset;
}

void SetCurrentMempool(Memory * pool) {
    Pool = pool;
}

Memory * GetCurrentMempool(void) {
    return Pool;
}

BOOL IsFinished() {
    return Pool != 0;
}

u32 GetCommonBlockSpaceFree() {
    return COMMON_BLOCK_SIZE - Bytes_used;
}

void * AllocateInCommonBlock(u32 size) {
    u32 alignedAmount;
    char* allocatedAddress;
    volatile s32* CurrentAllocation;

    alignedAmount = OSRoundUp32B(size);
    if (Common_block_allocation_amount[Common_block_index] != -1) {
        DebugError("memory.cpp", 449, "Common block is already locked.");
    }
    if (alignedAmount > 72800 - Bytes_used) {
        DebugError( "memory.cpp", 452, "Not enough space in common block.\n");
    }
    
    CurrentAllocation = Common_block_allocation_amount + Common_block_index;
    Common_block_index = Common_block_index + 1;
    allocatedAddress = COMMON_BLOCK + Bytes_used;
    *CurrentAllocation = alignedAmount;
    Bytes_used = Bytes_used + alignedAmount;
    return allocatedAddress;
}

void SetSomethingCommonBlock(u32 amount) {
    int unk;
    if (amount != 0) {
        Common_block_index--;
        unk = Common_block_allocation_amount[Common_block_index];
        Common_block_allocation_amount[Common_block_index] = -1;
        Bytes_used = Bytes_used - unk;
    }
}
