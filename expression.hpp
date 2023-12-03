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

    int BasicInitialise (int pType, int pID, std::string pValue);
    int Initialise (Token InputToken);
    int InitialiseWithTokens (Token InputToken, std::vector<Token> tokenVector);  

    int InitialiseWithArguments (Token InputToken, std::vector<ExpressionNode> ExprNodeVector);  

    std::string GetString();
    ~ExpressionNode();  
};


class Expression {
private:


public:
    std::vector<ExpressionNode> nodes;
    ~Expression();
    int InitialiseWithTokens(const std::vector<Token> tokenVector);
    int AddExpression(Expression &Expr);
    int AddNode(ExpressionNode &ExprNode);
    std::string GetString();
};

int TokensToExpressionCollection( std::vector<Token> pTokens, std::vector<Expression> &pLexResults);


#endif // EXPRESSION_HPP

