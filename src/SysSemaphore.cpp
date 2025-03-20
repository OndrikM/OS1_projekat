#include "../h/SysSemaphore.hpp"
#include "../h/syscall_c.hpp"

int SysSemaphore::wait(){
    if(count==0){
        blocked->add(ThreadCB::runningThread);
        ThreadCB::runningThread->setBlocked(true);
        thread_dispatch();
    }else{
        count--;
    }
    if(ThreadCB::runningThread->getFromSemClose()){
        ThreadCB::runningThread->setFromSemClose(false);
        return -1;
    }else{
        return 0;
    }
}

int SysSemaphore::signal(){
    if(blocked->getFirst()!=nullptr){
        ThreadCB* t=blocked->removeFirst();
        if(t!=nullptr){
            t->setBlocked(false);
            Scheduler::getInstance()->put(t);
        }
    }else{
        count++;
    }
    return 0;
}

int SysSemaphore::close(){
    for(ThreadCB* t=blocked->removeFirst();t!=nullptr;t=blocked->removeFirst()){
        t->setBlocked(false);
        t->setFromSemClose(true);
        Scheduler::getInstance()->put(t);
    }
    delete blocked;
    return 0;
}
//----------------------------------------------------------------------------------------------------------------------
int SysSemaphore::scSemaphoreOpen(SysSemaphore** handle, unsigned int init) {
    *handle = new SysSemaphore(init);
    if(*handle==nullptr)return -1;
    return 0;
}

int SysSemaphore::scSemaphoreClose(SysSemaphore* handle) {
    return handle->close();
}

int SysSemaphore::scWait(SysSemaphore *id) {
    return id->wait();
}

int SysSemaphore::scSignal(SysSemaphore *id) {
    return id->signal();
}

int SysSemaphore::scTryWait(SysSemaphore* id) {
    if(id->count<=0)return 1;
    if(--id->count==0)return 0;
    return 2;
}
