#ifndef _sysSemaphore_hpp_
#define _sysSemaphore_hpp_
#include "ThreadCB.hpp"
#include "ThreadList.hpp"
#include "RiscV.hpp"

class SysSemaphore{
private:
    int count;
    ThreadList<ThreadCB>* blocked;

public:
    SysSemaphore(int count=1):count(count){
        blocked = new ThreadList<ThreadCB>();
    }
    int wait();
    int signal();
    int close();

    //------------------------------------------------------------------------------------------------------------------
    static int scSemaphoreOpen(SysSemaphore** handle, unsigned init);
    static int scSemaphoreClose(SysSemaphore* handle);
    static int scWait(SysSemaphore* id);
    static int scSignal(SysSemaphore* id);
    static int scTryWait(SysSemaphore* id);
};

typedef SysSemaphore* sem_t;

#endif