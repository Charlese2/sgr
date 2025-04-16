#include "dolphin/types.h"
class MemSystem {
    public:
    void* unk0;
    int heapHandle;
};

inline u32 GetHeapHandle(MemSystem * memSystem) {
    return memSystem->heapHandle;
}

extern MemSystem gMemSystem;
