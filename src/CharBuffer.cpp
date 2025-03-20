#include "../h/CharBuffer.hpp"
#include "../h/MemoryAllocator.hpp"

CharBuffer::CharBuffer(int _cap) : cap(_cap + 1), head(0), tail(0) {
    buffer = (int *)MemoryAllocator::mem_alloc(sizeof(int) * cap);
    itemAvailable=new SysSemaphore(0);
    spaceAvailable=new SysSemaphore(_cap);
    mutexHead=new SysSemaphore(1);
    mutexTail=new SysSemaphore(1);
}

CharBuffer::~CharBuffer() {
    delete buffer;
    delete itemAvailable;
    delete spaceAvailable;
    delete mutexTail;
    delete mutexHead;
}

void CharBuffer::put(int val) {
    spaceAvailable->wait();

    mutexTail->wait();
    buffer[tail] = val;
    tail = (tail + 1) % cap;
    mutexTail->signal();

    itemAvailable->signal();

}

int CharBuffer::get() {
    itemAvailable->wait();

    mutexHead->wait();

    int ret = buffer[head];
    head = (head + 1) % cap;
    mutexHead->signal();

    spaceAvailable->signal();

    return ret;
}

int CharBuffer::getCnt() {
    int ret;

    mutexHead->wait();
    mutexTail->wait();

    if (tail >= head) {
        ret = tail - head;
    } else {
        ret = cap - head + tail;
    }

    mutexTail->signal();
    mutexHead->signal();

    return ret;
}
