#ifndef _syscall_cpp
#define _syscall_cpp
#include "syscall_c.hpp"
/*
void* ::operator new (size_t size){
    return mem_alloc(size);
}

void ::operator delete (void* p){
    mem_free(p);
}
*/
class Thread {
public:
    Thread (void (*body)(void*), void* arg):myHandle(nullptr),body(body),arg(arg) {};
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    static int sleep (time_t);
protected:
    Thread ():myHandle(nullptr),body(nullptr),arg(nullptr) {};
    virtual void run () {}
private:
    thread_t myHandle;
    void (*body)(void*); void* arg;
    static void wrapper (void* arg);
};

class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
    int timedWait (time_t);
    int tryWait();
private:
    sem_t myHandle;
};


class PeriodicThread : public Thread {
public:
    void terminate ();
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
private:
    time_t period;
};

class Console {
public:
    static char getc (){return __getc();};
    static void putc (char c){ __putc(c);};
};

#endif