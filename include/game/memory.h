#include "dolphin/types.h"

extern "C" {
    #include "dolphin/os/OSAlloc.h"
}

#define COMMON_BLOCK_SIZE 72800
#define MAX_POOL_NAME_LENGTH 16

typedef struct {
    char* destination;
    u32 size;
    u32 offset;
    u8 alignment;
    char pool_name[MAX_POOL_NAME_LENGTH];
} Mempool;

void* operator new(size_t amount, char * file, int line);
void* operator new[](size_t amount, char * file, int line);

void operator delete(void * p) throw();
void operator delete[](void * p) throw ();

void Copy(Mempool * mem_pool, char *destination, u32 size, char * name, u8 alignment);
void* AllocateInPool(Mempool* pool, u32 size);

void set_current_mempool(Mempool * memory);
Mempool* get_current_mempool(void);

BOOL is_mempool_active();

u32 GetCommonBlockSpaceFree();
void* AllocateInCommonBlock(u32 amount);
void SetSomethingCommonBlock(u32 amount);
