#include "game/memory.h"
#include "game/gamemem.h"
#include "game/debug.h"
#include "dolphin/os.h"
#include "game/macros.h"

typedef u8 ubyte;

u32 current_allocations;
u32 new_called;
u32 delete_called;
u32 highest_allocations;
u32 current_allocation_amount;
u32 highest_allocation_amount;

bool gHeapAlloc;
int memory1;
int memory2;
int memory3;
int memory4;
int memory5;
int memory6;
int memory7;
int memory8;
Mempool * Pool;
bool allocation_done;
u32 Common_block_index;
u32 Bytes_used;
u8 COMMON_BLOCK[COMMON_BLOCK_SIZE];
const char empty[4000] = "";
s32 Common_block_allocation_size[2] = {-1, -1};
extern char string_buffer[512];
volatile extern OSHeapHandle __OSCurrHeap;

void set_allocation_done(void) {
    if (!allocation_done) {
        allocation_done = true;
    }
}

void * operator new(size_t size, char* file, int line) {
    void* address;
    u32 t;
    char stringbuf[128];
    if (Pool) {
        return allocate_in_mempool(Pool, size);
    } else {
        DEBUGASSERTLINE(100, gHeapAlloc == true);

        DEBUGASSERTLINE(102, size > 0);

        address = OSAllocFromHeap(__OSCurrHeap, size);
        if ((void*)address == 0) {
            sprintf(stringbuf, "Failed to allocate %d bytes\n", size);
            DEBUGERRORLINE(108, stringbuf);
            return NULL;
        } else {
            t = (u32)address;
            DEBUGASSERTLINE(114, (t & 15) == 0);
#ifdef DEBUG
            if (!allocation_done) {
                set_allocation_done();
            }
            new_called++;
            current_allocations++;
            if (current_allocations > highest_allocations) {
                highest_allocations = current_allocations;
            }
            current_allocation_amount += size;
            if (current_allocation_amount > highest_allocation_amount) {
                highest_allocation_amount = current_allocation_amount;
            }
#endif
        }
    }
    return address;
}

void* operator new [](size_t size, char* file, int line) {
    return ::operator new(size, file, line);
}

void operator delete(void * p) throw () {
    DEBUGASSERTLINE(193, p != NULL);
    DEBUGASSERTLINE(194, Pool == NULL);
#ifdef DEBUG
    delete_called++;
    current_allocations--;
#endif
    OSFreeToHeap(__OSCurrHeap, p);
}


void operator delete[](void * p) throw () {
    ::operator delete (p);
}

void Copy(Mempool* mem_pool, u8* destination, u32 size, char* _name, u8 alignment) {
    DEBUGASSERTLINE(353, strlen(_name) < MAX_POOL_NAME_LENGTH);
    strcpy(mem_pool->pool_name, _name);
    mem_pool->destination = destination;
    mem_pool->size = size;
    mem_pool->offset = 0;
    mem_pool->alignment = alignment;
    DEBUGASSERTLINE(359, (alignment == 4) || (alignment == 16) || (alignment == 32));
}

void* allocate_in_mempool(Mempool* mempool, u32 size) {
    u8* destination;
    u32 Alinged_size;

    Alinged_size = ~(mempool->alignment - 1) & (size + mempool->alignment - 1);
    if (mempool->offset + Alinged_size > mempool->size) {
#ifdef DEBUG
        sprintf(string_buffer, "Failed allocation of\n%d bytes in %s pool\n(max pool size is %d\nspace left is %d)\n",
            size, mempool->pool_name, mempool->size, getPoolSpaceLeft(mempool));
        DebugError("memory.cpp", 370, string_buffer);
#else
        return NULL;
#endif
    }

    destination = mempool->destination + mempool->offset;
    mempool->offset += Alinged_size;
    return destination;
}

void set_current_mempool(Mempool * pool) {
    Pool = pool;
}

Mempool * get_current_mempool(void) {
    return Pool;
}

BOOL is_mempool_active() {
    BOOL pool_active;

    if (Pool){
        pool_active = true;
    }
    else {
        pool_active = false;
    }
    return pool_active;
}

u32 GetCommonBlockSpaceFree() {
    return COMMON_BLOCK_SIZE - Bytes_used;
}

u8 * allocate_in_commonblock(u32 size) {
    u8* Allocation_position;

    size = OSRoundUp32B(size);

    DEBUGASSERTLINE(446, Common_block_index < 2);

    if (Common_block_allocation_size[Common_block_index] != -1) {
        DebugError("memory.cpp", 449, "Common block is already locked.");
    }
    if (size > COMMON_BLOCK_SIZE - Bytes_used) {
        DebugError( "memory.cpp", 452, "Not enough space in common block.\n");
    }

    Common_block_allocation_size[Common_block_index++] = size;
    Allocation_position = COMMON_BLOCK + Bytes_used;
    Bytes_used += size;
    return Allocation_position;
}

void deallocate_from_commonblock(char* p) {
    int size;
    if (p) {
        DEBUGASSERTLINE(467, Common_block_index > 0);

        Common_block_index--;
        size = Common_block_allocation_size[Common_block_index];
        Common_block_allocation_size[Common_block_index] = -1;
        DEBUGASSERTLINE(472, size != -1);

        Bytes_used -= size;
        DEBUGASSERTLINE(475, (ubyte *)p == COMMON_BLOCK + Bytes_used);

        if (Common_block_index == 0) {
            DEBUGASSERTLINE(477, Bytes_used == 0);
        }
    }
}
