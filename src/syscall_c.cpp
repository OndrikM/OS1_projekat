#include "../h/syscall_c.hpp"

uint64 universalSysCall(uint64 operation, uint64 arg1, uint64 arg2, uint64 arg3, uint64 arg4){
    uint64 ret;
    asm volatile("ecall");
    asm volatile ("mv %[ret], a0" : [ret] "=r"(ret));
    return ret;
}

void* mem_alloc (size_t size){
    return (void*)universalSysCall(0x01, size);
}

int mem_free (void* ptr){
    return (int)universalSysCall(0x02, (uint64)ptr);
}

int thread_create (thread_t* handle,void(*start_routine)(void*),void* arg){
    return (int)universalSysCall(0x11, (uint64)handle, (uint64)start_routine, (uint64)arg);
}

int thread_exit (){
    return (int)universalSysCall(0x12);
}

void thread_dispatch (){
    universalSysCall(0x13);
}

int sem_open (sem_t* handle,unsigned init){
    return (int)universalSysCall(0x21, (uint64)handle, (uint64)init);
}

int sem_close (sem_t handle){
    return (int)universalSysCall(0x22, (uint64)handle);
}

int sem_wait (sem_t id){
    return (int)universalSysCall(0x23, (uint64)id);
}

int sem_signal (sem_t id){
    return (int)universalSysCall(0x24, (uint64)id);
}

int sem_timedwait(sem_t id,time_t timeout){
    return (int)universalSysCall(0x25, (uint64)id, (uint64)timeout);
}

int sem_trywait(sem_t id){
    return (int)universalSysCall(0x26, (uint64)id);
}

int time_sleep (time_t time){
    return (int) universalSysCall(0x31, (uint64)time);
}

char getc(){
    return (char) universalSysCall(0x41);
}

void putc(char c){
    universalSysCall(0x42, (uint64)c);
}