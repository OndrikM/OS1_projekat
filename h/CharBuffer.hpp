#ifndef charBufferHPP
#define charBufferHPP

#include "SysSemaphore.hpp"

class CharBuffer {
private:
    int cap;
    int *buffer;
    int head, tail;

    SysSemaphore* spaceAvailable;
    SysSemaphore* itemAvailable;
    SysSemaphore* mutexHead;
    SysSemaphore* mutexTail;

public:
    CharBuffer(int _cap=8192);
    ~CharBuffer();

    void put(int val);
    int get();

    int getCnt();

};

#endif