#include "dolphin/os/OSException.h"
#include "dolphin/types.h"

extern "C" {
    #include "dolphin/os/OSAlloc.h"
}

#define COMMON_BLOCK_SIZE 72800

volatile extern OSHeapHandle __OSCurrHeap;


typedef struct Memory {
    char * destination;
    u32 size;
    u32 offset;
    u8 alignment;
    char pool_name[19];
    u32 * unk20;
} _Memory;

void * Allocate(size_t amount, const char* file, int line);

void * operator new(size_t amount, const char * file, int line);
void * operator new[](size_t amount, const char * file, int line);

void operator delete(void * memoryAddress) throw();
void operator delete[](void * memoryAddress) throw ();

void copy(char *destination, u32 size, char * name, u8 alignment);
void * getOffset(u32 unk);

BOOL isFinished();

void SetCurrentMempool(Memory * memory);
Memory * GetCurrentMempool(void);

u32 getCommonBlockSpaceFree();
void * AllocateInCommonBlock(u32 amount);
void setSomethingCommonBlock(u32 amount);
