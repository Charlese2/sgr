#include "dolphin/types.h"
class MemSystem {
    public:
    virtual void unk0();
    int heapHandle;

    MemSystem();
    ~MemSystem();
};

inline u32 GetHeapHandle(MemSystem * memSystem) {
    return memSystem->heapHandle;
}

extern MemSystem gMemSystem;
