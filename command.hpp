#ifndef COMMAND_H_
#define COMMAND_H_

#include <string>
#include <vector>
#include "token.hpp"

class Command {
public:
    ~Command();
    int Initialise(std::vector<Token> &pTokens);
    std::string GetString();
    // int Execute();
private:
    int Type;
    int ID;
    std::vector<Token>  Parameters;
};

#endif /* COMMAND_H_ */