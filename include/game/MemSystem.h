#include "dolphin/types.h"
class MemSystem {
    virtual ~MemSystem();
    public:
    int heapHandle;

    MemSystem();
};

inline u32 GetHeapHandle(MemSystem * memSystem) {
    return memSystem->heapHandle;
}

extern MemSystem gMemSystem;
