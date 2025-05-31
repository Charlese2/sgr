#ifndef _H_GAME_MEMORY_
#define _H_GAME_MEMORY_

#include "dolphin/types.h"
#include "dolphin/os.h"

#define COMMON_BLOCK_SIZE    72800
#define MAX_POOL_NAME_LENGTH 16

typedef class Mempool {
  public:
    u8 *destination;
    u32 size;
    u32 offset;
    u8 alignment;
    char pool_name[MAX_POOL_NAME_LENGTH];
    Mempool() {};
    void Copy(u8 *_destination, u32 _size, char *_name, u8 _alignment);
} Mempool;

void set_allocation_done();

void *operator new(size_t amount, char *file, int line);
void *operator new[](size_t amount, char *file, int line);

void operator delete(void *p) throw();
void operator delete[](void *p) throw();

void *allocate_in_mempool(Mempool *pool, u32 size);

void set_current_mempool(Mempool *memory);
Mempool *get_current_mempool(void);

BOOL is_mempool_active();

u32 GetCommonBlockSpaceFree();
u32 *allocate_in_commonblock(u32 amount);
void deallocate_from_commonblock(u32 *p);

#endif // _H_GAME_MEMORY_
