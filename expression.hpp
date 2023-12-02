#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "token.hpp"
#include <vector>
#include <string>

bool IsTokenOKForExpression(const Token &token);


class ExpressionNode {
public:
    int Type;
    int ID;
    std::string Value;
    std::vector<ExpressionNode> Arguments;

    int Initialise (Token InputToken);
    int InitialiseWithArguments (Token InputToken, std::vector<ExpressionNode> InputArguments);  
    ~ExpressionNode();  
};


class Expression {
private:
    std::vector<ExpressionNode> nodes;

public:
    ~Expression();
    int Initialise(const std::vector<Token> tokenVector);
    std::string GetString();
};

#endif // EXPRESSION_HPP

