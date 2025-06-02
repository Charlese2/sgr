#include "dolphin/types.h"
class MemSystem {
  public:
    MemSystem();
    virtual ~MemSystem();
    u32 GetHeapHandle() { return heapHandle; }

  private:
    int heapHandle;
};

extern MemSystem gMemSystem;
