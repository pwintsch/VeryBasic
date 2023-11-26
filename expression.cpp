#include "expression.hpp"
#include "error.hpp"
#include "syntax.hpp"

int Expression::Initialise(const std::vector<Token>& tokenVector) {
    tokens = tokenVector;
    return NO_ERROR;
}

Expression::~Expression() {
    tokens.clear();
}