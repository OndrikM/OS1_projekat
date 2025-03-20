#include "../h/syscall_c.hpp"
#include "../h/MemoryAllocator.hpp"

#include "../lib/mem.h"

void* operator new(uint64 size){
    return mem_alloc(size);
    //return MemoryAllocator::mem_alloc(size);
}

void* operator new[](uint64 size){
    return mem_alloc(size);
    //return MemoryAllocator::mem_alloc(size);
}

void operator delete(void* p){
    mem_free(p);
    //MemoryAllocator::mem_free(p);
}

void operator delete[](void* p){
    mem_free(p);
    //MemoryAllocator::mem_free(p);
}
