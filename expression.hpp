#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "token.hpp"
#include <vector>

class Expression {
private:
    std::vector<Token> tokens;

public:
    ~Expression();
    int Initialise(const std::vector<Token>& tokenVector);
};

#endif // EXPRESSION_HPP

