#ifndef _schedulerHPP
#define _schedulerHPP
#include "./ThreadList.hpp"

class ThreadCB;

class Scheduler{
private:
    static Scheduler* handle;
    ThreadList<ThreadCB>* buffer;
    Scheduler(){
        this->buffer=new ThreadList<ThreadCB>();
    }
public:
    Scheduler(const Scheduler& obj)=delete;
    static Scheduler* getInstance(){
        if(handle==nullptr){
            handle=new Scheduler();
        }
        return handle;
    }
    void put(ThreadCB* thread);

    ThreadCB* get();

};

#endif