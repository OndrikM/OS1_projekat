#ifndef consoleHPP
#define consoleHPP

#include "CharBuffer.hpp"

class Console {
private:
    static bool userProgramFinished;
    static CharBuffer* input;
    static CharBuffer* output;
    Console(){}
public:
    Console(const Console& obj)=delete;

    static void init();
    static void endUserProgram();

    static void threadForOutput(void*);
    static void interruptForInput();

    static void scPutc(char c);
    static char scGetc();

    static void scPutcNonBuffered(char c);

};

#endif