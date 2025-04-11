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

char* Allocate(size_t size, char* file, int line) {
    int t;
    char stringbuf[128];
    if (Pool) {
        t = (int)AllocateInPool(Pool, size);
    } else {
        DEBUGASSERTLINE(100, gHeapAlloc == true);
        DEBUGASSERTLINE(102, size > 0);
        
        t = (int)OSAllocFromHeap(__OSCurrHeap, size);
        if ((void*)t == NULL) {
            sprintf(stringbuf, "Failed to allocate %d bytes\n", size);
            DEBUGERRORLINE(108, stringbuf);
            t = 0;
        } else {
            DEBUGASSERTLINE(114, (t & 15) == 0);
        }
    }
    return (char*)t;
}

char* AllocateArray(size_t size, char* file, int line) {
    return Allocate(size, file, line);
}

void Free(void * p) throw () {
    DEBUGASSERTLINE(193, p != NULL);
    DEBUGASSERTLINE(194, Pool == NULL);
    OSFreeToHeap(__OSCurrHeap, p);
}


void FreeArray(void * p) throw () {
    Free(p);
}

void Copy(Memory* mem_pool,  char* destination, u32 size, char* _name, u8 alignment) {
    DEBUGASSERTLINE(353, strlen(_name) < MAX_POOL_NAME_LENGTH);
    strcpy(mem_pool->pool_name, _name);
    mem_pool->destination = destination;
    mem_pool->size = size;
    mem_pool->offset = 0;
    mem_pool->alignment = alignment;
    DEBUGASSERTLINE(359, (alignment == 4) || (alignment == 16) || (alignment == 32));
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

void set_current_mempool(Memory * pool) {
    Pool = pool;
}

Memory * get_current_mempool(void) {
    return Pool;
}

BOOL is_mempool_active() {
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

    DEBUGASSERTLINE(446, Common_block_index < 2);

    if (Common_block_allocation_amount[Common_block_index] != -1) {
        DebugError("memory.cpp", 449, "Common block is already locked.");
    }
    if (alignedAmount > COMMON_BLOCK_SIZE - Bytes_used) {
        DebugError( "memory.cpp", 452, "Not enough space in common block.\n");
    }
    
    CurrentAllocation = Common_block_allocation_amount + Common_block_index;
    Common_block_index += 1;
    allocatedAddress = COMMON_BLOCK + Bytes_used;
    *CurrentAllocation = alignedAmount;
    Bytes_used += alignedAmount;
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
