#include "dolphin/types.h"
class MemSystem {
  public:
    MemSystem();
    virtual ~MemSystem();
    void CreateHeap();
    u32 GetHeapHandle() { return m_heapHandle; }

  private:
    int m_heapHandle;
};

extern MemSystem gMemSystem;
