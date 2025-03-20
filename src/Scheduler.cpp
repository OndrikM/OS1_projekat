#include "../h/Scheduler.hpp"

Scheduler* Scheduler::handle=nullptr;

void Scheduler::put(ThreadCB* t){
    this->buffer->add(t);
}

ThreadCB* Scheduler::get(){
    return this->buffer->removeFirst();
}
