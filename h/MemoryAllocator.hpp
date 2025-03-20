#ifndef _memoryAlocator
#define _memoryAlocator

#include "../lib/hw.h"

struct BlockHeader{
    size_t size;
    BlockHeader* next;
};

class MemoryAllocator{
public:
    static void init();
    static void* mem_alloc(size_t size);
    static int mem_free(void* ptr);
private:
    static BlockHeader* freeHead;
    static BlockHeader* usedHead;
    static void mergeBlocks(BlockHeader* first);
};


#endif