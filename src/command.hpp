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
    int BasicInitialise (int pType, int pID, std::string pValue);
    int InitialiseFromToken(Token &SourceToken);
    int InitialiseFromCommandNode(CommandNode &SourceNode);
    int InitialiseExpression(std::vector<CommandNode> &pArguments);
    int InitialiseWithArguments(const Token &SourceToken, std::vector<CommandNode> &pArguments);
    int InitialiseAsExpressionWithTokens (std::vector<Token> tokenVector);
    int NodeCount();
    int Evaluate(int &ResultType, float &NumResult, std::string &StrResult);
    int CheckExpressionReturnType(int &ResultType);
    int Precedence();
    std::string GetString();
    std::string GetDetailedString(int Padding); 
    int Type;
    int ID;
    std::string Value;
    std::vector<CommandNode> SubArguments;
};


class Command {
public:
    ~Command();
    int Initialise(std::vector<Token> &pTokens);
    int Lexerize (std::vector<Token> &Tokens, std::vector<CommandNode> &LexResults);
    int FindSyntaxRule(std::vector<CommandNode> &LexResults);
    std::string GetString();
    std::string GetDetailedString();
    int NodeCount();
    int Type;
    int ID;
    int RuleNo;
    std::vector<CommandNode>  Arguments;
    // int Execute();
private:

};




#endif /* COMMAND_H_ */