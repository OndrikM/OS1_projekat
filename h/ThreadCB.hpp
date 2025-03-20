#ifndef _threadHPP
#define _threadHPP
#include "../lib/hw.h"
#include "./Scheduler.hpp"
#include "./RiscV.hpp"
#define STACK_SIZE 4096

typedef ThreadCB* thread_t;

using runF = void(*)(void*);

class ThreadCB{
    friend class RiscV;
private:
    static int threadCount;
    int id;

    explicit ThreadCB(runF body, void* arg);
    runF body;void* arg;uint64* stack;
    struct Context{uint64 ra;uint64 sp;};
    Context context;bool finished;bool blocked;
    bool fromSemClose;

    static void contextSwitch(Context* oldContext, Context* newContext);
    static void dispatch();
    static void threadWrapper();

    bool isMain;

public:
    ~ThreadCB(){delete[] stack;}
    bool isFinished(){return finished;}
    void setFinished(bool value){finished = value;}
    bool isBlocked(){return blocked;}
    void setBlocked(bool value){blocked = value;}
    static ThreadCB* createThread(runF body, void* arg);
    void start(){Scheduler::getInstance()->put(this);}
    static void yield();
    static ThreadCB* runningThread;
    int getID(){return id;}
    void setFromSemClose(bool value){fromSemClose = value;}
    bool getFromSemClose(){return fromSemClose;}

    static int scThreadCreate(thread_t* handle,void(*start_routine)(void*),void* arg);
    static int scThreadExit();
};




#endif