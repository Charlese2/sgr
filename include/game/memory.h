#include "dolphin/types.h"

extern "C" {
    #include "dolphin/os/OSAlloc.h"
}

#define COMMON_BLOCK_SIZE 72800

typedef struct {
    char * destination;
    u32 size;
    u32 offset;
    u8 alignment;
    char pool_name[19];
    char * data;
} Memory;

char* Allocate(size_t amount, const char * file, int line);
char* AllocateArray(size_t amount, const char * file, int line);

void Free(void * memoryAddress) throw();
void FreeArray(void * memoryAddress) throw ();

void copy(Memory * mem_pool, char *destination, u32 size, char * name, u8 alignment);
char* allocateInPool(Memory* pool, u32 size);

BOOL isFinished();

void SetCurrentMempool(Memory * memory);
Memory * GetCurrentMempool(void);

u32 getCommonBlockSpaceFree();
void * AllocateInCommonBlock(u32 amount);
void setSomethingCommonBlock(u32 amount);
