#ifndef _riscV
#define _riscV
#include "../lib/hw.h"

class RiscV{
public:
    static void initVectorTable();

    static void trap();

    static void sysHandler(uint64 operation, uint64 arg1=0, uint64 arg2=0, uint64 arg3=0, uint64 arg4=0);

    enum Scause{
        TIMER_INTERRUPT = (1ul << 63) | 1,
        CONSOLE_INTERRUPT = (1ul << 63) | 9,
        ILLEGAL_INSTRUCTION = 2,
        ILLEGAL_ADDRESS_READ = 5,
        ILLEGAL_ADDRESS_WRITE = 7,
        ECALL_USER_MODE= 8,
        ECALL_SUPERVISOR_MODE= 9
    };

    static void popSppSpie();

    // read register scause
    static uint64 read_scause();

    // write register scause
    static void write_scause(uint64 scause);

    // read register sepc
    static uint64 read_sepc();

    // write register sepc
    static void write_sepc(uint64 sepc);

    // read register stvec
    static uint64 read_stvec();

    // write register stvec
    static void write_stvec(uint64 stvec);

    // read register stval
    static uint64 read_stval();

    // write register stval
    static void write_stval(uint64 stval);

    enum BitMaskSip {
        SIP_SSIE = (1 << 1),
        SIP_STIE = (1 << 5),
        SIP_SEIE = (1 << 9),
    };

    // mask set register sip
    static void ms_sip(uint64 mask);

    // mask clear register sip
    static void mc_sip(uint64 mask);

    // read register sip
    static uint64 read_sip();

    // write register sip
    static void write_sip(uint64 sip);

    enum BitMaskSstatus {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8),
    };

    // mask set register sstatus
    static void ms_sstatus(uint64 mask);

    // mask clear register sstatus
    static void mc_sstatus(uint64 mask);

    // read register sstatus
    static uint64 read_sstatus();

    // write register sstatus
    static void write_sstatus(uint64 sstatus);

    static uint64 read_sscratch();


};

inline void RiscV::initVectorTable() {
    asm volatile("csrw stvec, %[vector]" : : [vector] "r" (&RiscV::trap));
}

inline uint64 RiscV::read_sscratch() {
    uint64 volatile sscratch;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(sscratch));
    return sscratch;
}

inline uint64 RiscV::read_scause() {
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline void RiscV::write_scause(uint64 scause) {
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 RiscV::read_sepc() {
    uint64 volatile sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}

inline void RiscV::write_sepc(uint64 sepc) {
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 RiscV::read_stvec() {
    uint64 volatile stvec;
    __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
    return stvec;
}

inline void RiscV::write_stvec(uint64 stvec) {
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 RiscV::read_stval() {
    uint64 volatile stval;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(stval));
    return stval;
}

inline void RiscV::write_stval(uint64 stval) {
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline void RiscV::ms_sip(uint64 mask) {
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sip(uint64 mask) {
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscV::read_sip() {
    uint64 volatile sip;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(sip));
    return sip;
}

inline void RiscV::write_sip(uint64 sip) {
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void RiscV::ms_sstatus(uint64 mask) {
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sstatus(uint64 mask) {
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscV::read_sstatus() {
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

inline void RiscV::write_sstatus(uint64 sstatus) {
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

#endif