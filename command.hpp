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
    int InitialiseFromCommandNode(CommandNode &SourceNode);
    int InitialiseExpression(int pType, int pID, Expression &pExpression);
    int InitialiseWithArguments(const Token &SourceToken, std::vector<Expression> &pArguments);
    std::string GetString();  
    int Type;
    int ID;
    std::string Value;
    Expression CmdExpr;
    std::vector<Expression> SubArguments;
};


class Command {
public:
    ~Command();
    int Initialise(std::vector<Token> &pTokens);
    int Lexerize (std::vector<Token> &pTokens, std::vector<CommandNode> &LexResults);
    int FindSyntaxRule(std::vector<CommandNode> &LexResults);
    std::string GetString();
    std::string GetDetailedString();
    int Type;
    int ID;
    int RuleNo;
    std::vector<CommandNode>  Arguments;
    // int Execute();
private:

};




#endif /* COMMAND_H_ */