#ifndef _syscall_c
#define _syscall_c
#include "../lib/hw.h"
#include "./ThreadCB.hpp"
#include "./SysSemaphore.hpp"

#include "../lib/console.h"

uint64 universalSysCall(uint64 operation, uint64 arg1=0, uint64 arg2=0, uint64 arg3=0, uint64 arg4=0);

void* mem_alloc (size_t size); //size je u bajtovima

int mem_free (void* ptr);
//----------------------------------------------------------------------------------------------------------------------
typedef ThreadCB* thread_t;

int thread_create (thread_t* handle,void(*start_routine)(void*),void* arg);

int thread_exit ();

void thread_dispatch ();
//----------------------------------------------------------------------------------------------------------------------
typedef SysSemaphore* sem_t;

int sem_open (sem_t* handle,unsigned init);

int sem_close (sem_t handle);

int sem_wait (sem_t id);

int sem_signal (sem_t id);

int sem_timedwait(sem_t id,time_t timeout); //nije implementirano, u okviru asinhrone promene

int sem_trywait(sem_t id);
//----------------------------------------------------------------------------------------------------------------------
typedef unsigned long time_t;

int time_sleep (time_t);
//----------------------------------------------------------------------------------------------------------------------
const int EOF = -1;

char getc();

void putc(char c);
//----------------------------------------------------------------------------------------------------------------------


#endif