#include "dolphin/os/OSException.h"
#include "dolphin/types.h"

extern "C" {
    #include "dolphin/os/OSAlloc.h"
}

volatile extern OSHeapHandle __OSCurrHeap;

class Memory {
    public:

    typedef struct MemoryStruct {
        char * unk0;
        u32 unk4;
        u32 offset;
        u8 remaining;
        char name[19];
        u32 * unk20;
    } _MemoryStruct;

    void * operator new(size_t amount, const char * file, int line);
    void * operator new[](size_t amount, const char * file, int line);

    void operator delete(void * memoryAddress) throw();
    void operator delete[](void * memoryAddress) throw ();

    static void SetSomethingMemory(MemoryStruct * unk);
    static MemoryStruct * GetSomethingMemory(void);

    void * AllocateInCommonBlock(u32 amount);

    private:

    static void * Allocate(size_t amount, const char* file, int line);

};
