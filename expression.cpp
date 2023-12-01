#include "expression.hpp"
#include "error.hpp"
#include "syntax.hpp"


bool IsTokenOKForExpression(const Token &token) {
    switch (token.Type)
    {
    case tValue:
    case tVariable:
    case tFunction:
    case tOperator:
    case tComparison:
        return true;
        break;
    case tBracket:
        if (token.ID == coOpenBracket || token.ID == coCloseBracket) {
            return true;
        } 
        break;
    
    default:
        break;
    }

    return false;
}


int Expression::Initialise(const std::vector<Token> tokenVector) {
    tokens = tokenVector;
    // check brackets are balanced
    return NO_ERROR;
}

Expression::~Expression() {
    tokens.clear();
}

std::string Expression::GetString() {
    std::string result;
    for (int i=0; i<tokens.size(); i++) {
        if (i>0) {
            result+=", ";
        }
        result+=tokens[i].Value;
    }
    return result;
}