#include "../h/RiscV.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/ThreadCB.hpp"
#include "../h/SysSemaphore.hpp"

#include "../test/printing.hpp"
#include "../lib/console.h"


void RiscV::popSppSpie() {
    RiscV::mc_sstatus(RiscV::SSTATUS_SPP);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void RiscV::sysHandler(uint64 operation, uint64 arg1, uint64 arg2, uint64 arg3, uint64 arg4) {
    uint64 sepc;
    uint64 sstatus;
    sepc=RiscV::read_sepc();
    sstatus=RiscV::read_sstatus();
    uint64 scause = RiscV::read_scause();

    uint64 ret=0;

    if (scause == ECALL_USER_MODE || scause == ECALL_SUPERVISOR_MODE){
        switch (operation) {
            case 0x01:ret=(uint64)MemoryAllocator::mem_alloc(arg1);break;
            case 0x02:ret=(uint64)MemoryAllocator::mem_free((void*)arg1);break;
            case 0x11:ret=(uint64)ThreadCB::scThreadCreate((thread_t*)arg1, (void(*)(void*))arg2, (void*)arg3);break;
            case 0x12:ret=(uint64)ThreadCB::scThreadExit();break;
            case 0x13:ret=0;ThreadCB::dispatch();break;
            case 0x21:ret=(uint64)SysSemaphore::scSemaphoreOpen((sem_t*)arg1, (unsigned)arg2);break;
            case 0x22:ret=(uint64)SysSemaphore::scSemaphoreClose((sem_t)arg1);break;
            case 0x23:ret=(uint64)SysSemaphore::scWait((sem_t)arg1);break;
            case 0x24:ret=(uint64)SysSemaphore::scSignal((sem_t)arg1);break;
            case 0x25:ret=0;break;//(uint64)SysSemaphore::scTimedWait((sem_t)arg1, (time_t)arg2);break;
            case 0x26:ret=(uint64)SysSemaphore::scTryWait((sem_t)arg1);break;

            case 0x31:ret=0;break;//(uint64)ThreadCB::scTimeSleep((time_t)arg1);break;

            case 0x41:ret=__getc();break;
            case 0x42:ret=0;__putc((char)arg1);break;
        }

        __asm__ volatile("mv t0, %0" : : "r"(ret));
        __asm__ volatile ("sw t0, 80(x8)");

        sepc=sepc+4;
        RiscV::write_sstatus(sstatus);
        RiscV::write_sepc(sepc);

    }
    else if (scause == TIMER_INTERRUPT){
        mc_sip(SIP_SSIE);
        /*TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }*/
        write_sstatus(sstatus);
        write_sepc(sepc);
    }
    else if (scause == CONSOLE_INTERRUPT){
        //Console::interruptForInput();
        //plic_complete(plic_claim());
        console_handler();
    }
    else if (scause == ILLEGAL_INSTRUCTION){
        printString("Greska, ilegalna instrukcija\n ");
        while(1);
    }
    else if(scause == ILLEGAL_ADDRESS_READ) {
        printString("Greska, nedozvoljena adresa citanja\n ");
        while (1);
    }
    else if(scause == ILLEGAL_ADDRESS_WRITE){
        printString("Greska, nedozvoljena adresa upisa\n ");
        while (1);
    }
    else{
        printString("Greska, nepoznat prekid\n ");
        while(1);
    }

}