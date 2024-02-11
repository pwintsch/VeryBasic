#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <string>
#include <vector>
#include "token.hpp"
#include "command.hpp"

class Instruction {
public:
    int ProgramLine;
    int CommandID;
    std::vector<Command> Commands;

    ~Instruction();
    int Initialise(TokenCollection &InputTokens);
    std::string GetString();
    std::string GetDetailedString();
    int NodeCount();


private:
    /* data */
    int AddCommand(std::vector<Token> pCommand);
};

#endif /* INSTRUCTION_H_ */

