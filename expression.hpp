#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "token.hpp"
#include <vector>
#include <string>

bool IsTokenOKForExpression(const Token &token);


class Expression {
private:
    std::vector<Token> tokens;

public:
    ~Expression();
    int Initialise(const std::vector<Token> tokenVector);
    std::string GetString();
};

#endif // EXPRESSION_HPP

