#include "../h/syscall_cpp.hpp"

int Thread::start() {
    if(body!=nullptr){
        return thread_create(&myHandle, body, arg);
    }else{
        return thread_create(&myHandle, Thread::wrapper, (void*)this);
    }
}

void Thread::wrapper(void* args) {
    ((Thread*)args)->run();
}

Thread::~Thread() {
    if(myHandle != nullptr) {
        delete myHandle;
    }
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t) {
    return 0;
}

Semaphore::Semaphore(unsigned init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    if(myHandle != nullptr) {
        delete myHandle;
    }
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}

