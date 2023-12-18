#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <vector>
#include <string>
#include "instruction.hpp"

class Processor {
private:
    std::vector<Instruction> Program;
public:
    Processor();
    ~Processor();

    bool Active=true;
    int Addline(Instruction MyInstruction);
    int ChangeLine(Instruction MyInstruction);
    int RemoveLine(int i);
    std::string ListFull();
    std::string ListPartial(int StartLineNo, int EndLineNo);
    std::string Listline(int LineNo);
    void Exit();
};

extern Processor MyProcessor;



#endif // PROCESSOR_HPP

