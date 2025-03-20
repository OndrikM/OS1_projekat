#include "../h/ThreadCB.hpp"
#include "../h/syscall_c.hpp"

int ThreadCB::threadCount = 0;
ThreadCB* ThreadCB::runningThread = nullptr;

void ThreadCB::threadWrapper() {
    RiscV::popSppSpie();
    ThreadCB::runningThread->body(ThreadCB::runningThread->arg);
    runningThread->setFinished(true);
    thread_dispatch();
}

ThreadCB::ThreadCB(runF body, void *arg) : id(threadCount++), body(body), arg(arg),
                                         stack(body!=nullptr ? new uint64[STACK_SIZE] : nullptr),
                                         context({(uint64)threadWrapper ,
                         stack!=nullptr  ? (uint64)&stack[STACK_SIZE] : 0}),
                                         finished(false),blocked(false),fromSemClose(false) {if (body==nullptr){ThreadCB::runningThread=this;isMain=true;}else{isMain=false;}}

ThreadCB* ThreadCB::createThread(runF body, void *arg) {
    return new ThreadCB(body, arg);
}

void ThreadCB::dispatch() {
    ThreadCB* old=runningThread;
    if(!old->isFinished() && !old->isBlocked()){Scheduler::getInstance()->put(old);}
    runningThread=Scheduler::getInstance()->get();
    ThreadCB::contextSwitch(&old->context, &runningThread->context);
}

void ThreadCB::yield() {
    thread_dispatch();
}

int ThreadCB::scThreadCreate(thread_t* handle, void (*start_routine)(void *), void *arg) {
    *handle = new ThreadCB(start_routine, arg);
    if(*handle!= nullptr){
        if(start_routine)Scheduler::getInstance()->put(*handle);
        return 0;
    }else{
        return -1;
    }
}

int ThreadCB::scThreadExit() {
    runningThread->setFinished(true);
    thread_dispatch();
    return 0;
}