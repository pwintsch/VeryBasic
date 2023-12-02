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


int ExpressionNode::Initialise (Token InputToken) {
    Type = InputToken.Type;
    ID = InputToken.ID;
    Value = InputToken.Value;
    return NO_ERROR;
}

int ExpressionNode::InitialiseWithArguments (Token InputToken, std::vector<ExpressionNode> InputArguments) {
    Type = InputToken.Type;
    ID = InputToken.ID;
    Value = InputToken.Value;
    Arguments = InputArguments;
    return NO_ERROR;
}



ExpressionNode::~ExpressionNode() {
    Arguments.clear();
}  





int Expression::Initialise(const std::vector<Token> tokenVector) {

    nodes.clear();
    for (int i=0; i<tokenVector.size(); i++) {
        ExpressionNode node;
        node.Initialise(tokenVector[i]);
        nodes.push_back(node);
    }
    // check brackets are balanced
    return NO_ERROR;
}

Expression::~Expression() {
    nodes.clear();
}

std::string Expression::GetString() {
    std::string result;
    for (int i=0; i<nodes.size(); i++) {
        if (i>0) {
            result+=", ";
        }
        result+=nodes[i].Value;
    }
    return result;
}