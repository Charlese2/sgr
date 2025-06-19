
#include "game/memory.h"
#include "dolphin/os.h"



class vmemory {
  public:
    static u32 PoolAllocate32() {
        Mempool *pool;
        u32 result;
        pool = get_current_mempool();
        if (!pool) {
            return 32;
        }
        else {
            result = (u32)allocate_in_mempool(pool, 0);
            return OSRoundUp32B(result) - result;
        }
    }
    static u8* Unknown(u32 requested_size, u32 *custom_destination) {
        u32 offset;
        u8 *destination;

        offset = PoolAllocate32();
        destination = NULL;
        if (offset > 31) {
            requested_size += 31;
            destination = NEW_ALLOCATION(109, u8[requested_size]);
            if (custom_destination) {
                *custom_destination = (u32)destination;
            }
            destination = (u8 *)OSRoundUp32B(destination);
        }
        else {
            requested_size += offset;
            destination = NEW_ALLOCATION(119, u8[requested_size]);
            if (custom_destination) {
                *custom_destination = (u32)destination;
            }
            destination = destination + offset;
        }
        return destination;
    }
};
