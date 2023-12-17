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

    int Addline(Instruction MyInstruction);
    int ChangeLine(Instruction MyInstruction);
    int RemoveLine(int i);
    std::string List();
};

extern Processor MyProcessor;



#endif // PROCESSOR_HPP

