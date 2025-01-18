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
    u32 * unk20;
} Memory;

void* Allocate(size_t amount, const char * file, int line);
void* AllocateArray(size_t amount, const char * file, int line);

void Free(void * memoryAddress) throw();
void FreeArray(void * memoryAddress) throw ();

void copy(char *destination, u32 size, char * name, u8 alignment);
void* allocateInPool(Memory* pool, u32 size);
void* getOffset(u32 unk);

BOOL isFinished();

void SetCurrentMempool(Memory * memory);
Memory * GetCurrentMempool(void);

u32 getCommonBlockSpaceFree();
void * AllocateInCommonBlock(u32 amount);
void setSomethingCommonBlock(u32 amount);
