#ifndef COMMAND_H_
#define COMMAND_H_

#include <string>
#include <vector>
#include "token.hpp"
#include "syntax.hpp"
#include "expression.hpp"

class CommandNode {
public:
    CommandNode();
    ~CommandNode();
    int InitialiseFromToken(Token &SourceToken);
    int InitialiseExpression(int pType, int pID, Expression &pExpression);
    std::string GetString();  
    int Type;
    int ID;
    std::string Value;
    Expression ExpressionNode;
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
    int RuleNo;
    std::vector<Token> Tokens;
    std::vector<CommandNode>  Arguments;
    std::vector<tSyntax> Rules;
};




#endif /* COMMAND_H_ */