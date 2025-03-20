#include "../h/MemoryAllocator.hpp"
#include "../h/RiscV.hpp"
#include "../h/ThreadCB.hpp"
#include "../h/Console.hpp"

#include "../h/syscall_c.hpp"
#include "../test/printing.hpp"

extern void userMain();

void userMainWrapper(void* arg){
    userMain();
}

int main() {
    MemoryAllocator::init();
    RiscV::initVectorTable();
    Console::init();

    ThreadCB* mainThread = ThreadCB::createThread(nullptr, nullptr);
    mainThread->isFinished();

    ThreadCB* userThread = ThreadCB::createThread(userMainWrapper, nullptr);
    userThread->start();

    printString("Kernel pokrenut\n");

    while(!userThread->isFinished()){
        thread_dispatch();
    }

    printString("Kernel zavrsio\n");

    return 0;

}


