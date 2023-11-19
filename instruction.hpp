#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include <string>
#include <vector>
#include "token.hpp"

class Instruction {
public:
    int ProgramLine;
    int CommandID;
    std::vector<std::vector <Token> > Parameters;

    int Initialise(TokenCollection &InputTokens);
    std::string GetString();
};

#endif /* INSTRUCTION_H_ */

