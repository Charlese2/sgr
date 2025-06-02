
#include "game/memory.h"

class vmemory {
  public:
    static u32 vMemoryUnknown(u32 test) {
        Mempool *pool;
        u32 result;
        pool = get_current_mempool();
        if (!pool) {
            return (u8)32;
        }
        else {
            result = (u32)allocate_in_mempool(pool, 0);
            return (result = result & ~0x1F) - (int)pool;
        }
    }
};
