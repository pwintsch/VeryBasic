#ifndef COMMAND_H_
#define COMMAND_H_

#include <string>
#include <vector>
#include "token.hpp"
#include "syntax.hpp"

class CommandNode {
public:
    CommandNode();
    ~CommandNode();
    int Initialise(Token &SourceToken);
    std::string GetString();
    // int Execute();
private:   
    int Type;
    int ID;
    std::string Value;
};


class Command {
public:
    ~Command();
    int Initialise(std::vector<Token> &pTokens);
    std::string GetString();
    // int Execute();
private:
    int Type;
    int ID;
    std::vector<Token> Tokens;
    std::vector<CommandNode>  Arguments;
    std::vector<tSyntax> Rules;
};




#endif /* COMMAND_H_ */