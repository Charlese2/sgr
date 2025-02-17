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
    char pool_name[19];
} Memory;

char* Allocate(size_t amount, const char * file, int line);
char* AllocateArray(size_t amount, const char * file, int line);

void Free(void * memoryAddress) throw();
void FreeArray(void * memoryAddress) throw ();

void Copy(Memory * mem_pool, char *destination, u32 size, char * name, u8 alignment);
char* AllocateInPool(Memory* pool, u32 size);

void set_current_mempool(Memory * memory);
Memory* get_current_mempool(void);

BOOL is_mempool_active();

u32 GetCommonBlockSpaceFree();
void* AllocateInCommonBlock(u32 amount);
void SetSomethingCommonBlock(u32 amount);
