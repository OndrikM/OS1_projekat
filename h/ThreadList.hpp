#ifndef _threadListHPP
#define _threadListHPP

template<typename T>
class ThreadList{
private:
    struct Node{
        T* thread;
        Node* next;
    };
    Node* head;
    Node* tail;
public:
    ThreadList(){
        head=nullptr;
        tail=nullptr;
    }

    void add(T* thread){
        Node* newNode=new Node();
        newNode->thread=thread;
        newNode->next=nullptr;
        if(head==nullptr){
            head=newNode;
            tail=newNode;
        }else{
            tail->next=newNode;
            tail=newNode;
        }
    }

    T* removeFirst(){
        if(head==nullptr){
            return nullptr;
        }
        Node* first=head;
        head=head->next;
        T* thread=first->thread;
        delete first;
        return thread;
    }

    T* getFirst(){
        if(head==nullptr){
            return nullptr;
        }
        return head->thread;
    }

    void remove(T* thread){
        Node* current=head;
        Node* previous=nullptr;
        while(current!=nullptr){
            if(current->thread==thread){
                if(previous==nullptr){
                    head=current->next;
                }else{
                    previous->next=current->next;
                }
                if(current==tail){
                    tail=previous;
                }
                delete current;
                return;
            }
            previous=current;
            current=current->next;
        }
    }

};
#endif