#include "../h/MemoryAllocator.hpp"

#include "../test/printing.hpp"

BlockHeader* MemoryAllocator::freeHead;
BlockHeader* MemoryAllocator::usedHead;

void MemoryAllocator::init() {
    uint64 alignedStartAdress;
    uint64 modulo = (uint64)HEAP_START_ADDR & ((uint64)MEM_BLOCK_SIZE-1);
    alignedStartAdress = modulo==0x0? (uint64)HEAP_START_ADDR: (uint64)HEAP_START_ADDR+(uint64)MEM_BLOCK_SIZE-modulo;

    freeHead=(BlockHeader*)alignedStartAdress;
    freeHead->size = ((uint64)HEAP_END_ADDR - alignedStartAdress)/(uint64)MEM_BLOCK_SIZE;
    usedHead=nullptr;
}

void* MemoryAllocator::mem_alloc(size_t size){
    size_t blocksToAlloc;
    if( (size + sizeof(BlockHeader)) % MEM_BLOCK_SIZE != 0) {
        blocksToAlloc = (size + sizeof(BlockHeader) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    }
    else {
        blocksToAlloc = (size + sizeof(BlockHeader)) / MEM_BLOCK_SIZE;
    }

    BlockHeader* currentBlock = freeHead;
    BlockHeader* prevBlock = nullptr;
    while(currentBlock != nullptr){
        if(currentBlock->size >= blocksToAlloc){
            if(currentBlock->size == blocksToAlloc){
                if(prevBlock == nullptr){
                    freeHead = currentBlock->next;
                }
                else{
                    prevBlock->next = currentBlock->next;
                }
            }
            else{
                BlockHeader* newBlock = (BlockHeader*)((uint64)currentBlock + blocksToAlloc*MEM_BLOCK_SIZE);
                newBlock->size = currentBlock->size - blocksToAlloc;
                newBlock->next = currentBlock->next;
                if(prevBlock == nullptr){
                    freeHead = newBlock;
                }
                else{
                    prevBlock->next = newBlock;
                }
            }
            currentBlock->size = blocksToAlloc;
            currentBlock->next = usedHead;
            usedHead = currentBlock;
            return (void*)((uint64)currentBlock+sizeof(BlockHeader));
        }
        prevBlock = currentBlock;
        currentBlock = currentBlock->next;
    }

    return nullptr;

}

int MemoryAllocator::mem_free(void* ptr){
    ptr=(void*)((uint64)ptr-sizeof(BlockHeader));

    if(ptr == nullptr){
        return -1;
    }

    BlockHeader* blockToFind=usedHead;
    while(blockToFind!= nullptr && (void*)((uint64)blockToFind) != ptr){
        blockToFind=blockToFind->next;
    }

    if(blockToFind==nullptr){
        return -1;
    }else{
        if(usedHead==blockToFind) {
            usedHead = blockToFind->next;
        }else{
            BlockHeader* currentBlock = usedHead;
            BlockHeader* prevBlock = nullptr;
            while(currentBlock != nullptr && currentBlock != blockToFind){
                prevBlock = currentBlock;
                currentBlock = currentBlock->next;
            }
            prevBlock->next = currentBlock->next;
        }

        BlockHeader* currentBlock = freeHead;
        //BlockHeader* prevBlock = nullptr;
        while(currentBlock != nullptr && currentBlock->next < blockToFind){
            //prevBlock = currentBlock;
            currentBlock = currentBlock->next;
        }

        if(freeHead==nullptr){
            blockToFind->next = nullptr;
            freeHead = blockToFind;
            return 0;
        }else if((char*)blockToFind< (char*)freeHead) {
            blockToFind->next = freeHead;
            freeHead = blockToFind;
            mergeBlocks(blockToFind);
            return 0;
        }else{
            blockToFind->next = currentBlock->next;
            currentBlock->next = blockToFind;
            mergeBlocks(blockToFind);
            mergeBlocks(currentBlock);
            return 0;
        }
    }

    return -1;
}

void MemoryAllocator::mergeBlocks(BlockHeader* first){
    if(first->next && ((char*)(first) + first->size * MEM_BLOCK_SIZE) == (char*)first->next) {
        first->size += first->next->size;
        first->next = first->next->next;
    }
}