#include "../h/Console.hpp"
#include "../lib/hw.h"
#include "../h/syscall_c.hpp"
#include "../test/printing.hpp"

CharBuffer* Console::input = nullptr;
CharBuffer* Console::output = nullptr;
bool Console::userProgramFinished=false;

void Console::init(){
    input=new CharBuffer();
    output=new CharBuffer();
}

void Console::endUserProgram() {
    Console::userProgramFinished=true;
}

void Console::threadForOutput(void * ) {
    while(true){
        if(output->getCnt()==0 && userProgramFinished){
            thread_exit();
        }

        uint8 status=*((uint8*)CONSOLE_STATUS);
        bool ready=status&CONSOLE_TX_STATUS_BIT;
        if(ready && output->getCnt()>0){
            char data=output->get();
            *(char*)CONSOLE_TX_DATA=data;
        }else{
            thread_dispatch();
        }
    }
}

void Console::interruptForInput() {
    uint8 status=*((uint8*)CONSOLE_STATUS);
    bool ready=status&CONSOLE_RX_STATUS_BIT;
    if(ready){
        char data=*(char*)CONSOLE_RX_DATA;
        input->put(data);
    }
}

void Console::scPutc(char c) {
    output->put(c);
}

char Console::scGetc() {
    char c=input->get();
    return c;
}

void Console::scPutcNonBuffered(char c) {
    while(!(*((uint8*)CONSOLE_STATUS)&CONSOLE_TX_STATUS_BIT));
    *(char*)CONSOLE_TX_DATA=c;
}