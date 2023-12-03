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




int TokensToExpressionCollection( std::vector<Token> pTokens, std::vector<Expression> &pLexResults) {
    // Look for commas and split accordingly
    // ensure commas are not included in brackets
    int i=0;
    int LastStart=0;
    int BracketCount=0;
    pLexResults.clear();
    while (i<pTokens.size()) {
        if (IsTokenOKForExpression(pTokens[i])==false && pTokens[i].ID!=coComma) {
            return ERR_BAD_EXPRESSION;
        }
        if (pTokens[i].ID==coOpenBracket) {
            BracketCount++;
        } else if (pTokens[i].ID==coCloseBracket) {
            BracketCount--;
        } else if (pTokens[i].ID==coComma && BracketCount==0) {
            // create expression node from LastStart to i-1 and add to pLexResults
            Expression Expr;
            int r=Expr.InitialiseWithTokens(std::vector<Token>(pTokens.begin() + LastStart, pTokens.begin() + i));
            if (r!=NO_ERROR) {
                return r;
            }
            pLexResults.push_back(Expr);
            LastStart=i+1; 
        }
        // else if (pTokens[i].Type==tVariable && pTokens[i+1].ID==coOpenBracket) {
        // find matching close bracket
        // create Expresssion node of Function Type with arguments
        i++;
    }
    if (LastStart<i) {
        Expression Expr;
        int r=Expr.InitialiseWithTokens(std::vector<Token>(pTokens.begin() + LastStart, pTokens.begin() + i));
        if (r!=NO_ERROR) {
            return r;
        }
        pLexResults.push_back(Expr);
    }
   return NO_ERROR; 
}

// creates a vector of ExpressionNodes where each node is a paramater to a function
// start by looking for commas (not enclosed in brackets) and split accordingly
int TokensToExpressionNodeCollection( std::vector<Token> pTokens, std::vector<ExpressionNode> &pLexResults) {
    // Look for commas and split accordingly
    // ensure commas are not included in brackets
    int i=0;
    int LastStart=0;
    
    ExpressionNode NewNode;
    NewNode.Type=tExpression;
    NewNode.ID=0;
    NewNode.Value="";
    NewNode.Arguments.clear();
    pLexResults.clear();
    while (i<pTokens.size()) {
        if (IsTokenOKForExpression(pTokens[i])==false && pTokens[i].ID!=coComma) {
            return ERR_BAD_EXPRESSION;
        }
        if (pTokens[i].Type==tFunction) {
            // check for function arguments
            int bracketCount=0;
            int argumentStart=0;
            int argumentEnd=0;
            if (pTokens[i+1].Type==tBracket && pTokens[i+1].ID==coOpenBracket) {
                bracketCount++;
                argumentStart=i+2;
                argumentEnd=argumentStart;
                while (bracketCount>0 && argumentEnd<pTokens.size()) {
                    argumentEnd++;
                    if (pTokens[argumentEnd].ID==coOpenBracket) {
                        bracketCount++;
                    } else if (pTokens[argumentEnd].ID==coCloseBracket) {
                        bracketCount--;
                    }                    
                }
                if (bracketCount>0) {
                    return ERR_BAD_FUNCTION_BRACKET;
                }
                // arguments are in argumentStart to argumentEnd-1
                std::vector<Token> argumentTokens=std::vector<Token>(pTokens.begin()+argumentStart, pTokens.begin()+argumentEnd);
                std::vector<ExpressionNode> argumentNodes;
                int r=TokensToExpressionNodeCollection(argumentTokens, argumentNodes);
                if (r!=NO_ERROR) {
                    return r;
                }
                ExpressionNode node;
                int r2=node.InitialiseWithArguments(pTokens[i], argumentNodes);
                if (r2!=NO_ERROR) {
                    return r;
                }
                NewNode.Arguments.push_back(node);
                i=argumentEnd+1; // Here
            } else {
                // no arguments
                ExpressionNode node;
                int r=node.Initialise(pTokens[i]);
                if (r!=NO_ERROR) {
                    return r;
                }
                NewNode.Arguments.push_back(node);
                i++;
            }
        } else if (pTokens[i].ID!=coComma) {
            ExpressionNode node;
            int r=node.Initialise(pTokens[i]);
            if (r!=NO_ERROR) {
                return r;
            }
            NewNode.Arguments.push_back(node);
            i++;
        } else {
            // comma
            // create expression node from LastStart to i-1 and add to pLexResults
            pLexResults.push_back(NewNode);
            NewNode.Type=tExpression;
            NewNode.ID=0;
            NewNode.Value="";
            NewNode.Arguments.clear();
            i++;
        }
        // else if (pTokens[i].Type==tVariable && pTokens[i+1].ID==coOpenBracket) {
        // find matching close bracket
        // create Expresssion node of Function Type with arguments

    }
    if (NewNode.Arguments.size()>0) {
        pLexResults.push_back(NewNode);
    }
   return NO_ERROR; 
}

int ExpressionNode::BasicInitialise (int pType, int pID, std::string pValue) {
    Type=pType;
    ID=pID;
    Value=pValue;
    return NO_ERROR;
}

int ExpressionNode::Initialise (Token InputToken) {
    if (!IsTokenOKForExpression(InputToken)) {
        return ERR_BAD_EXPRESSION;
    }
    Type = InputToken.Type;
    ID = InputToken.ID;
    Value = InputToken.Value;
    return NO_ERROR;
}


int ExpressionNode::InitialiseWithArguments (Token InputToken, std::vector<ExpressionNode> ExprNodeVector) {
    if (!IsTokenOKForExpression(InputToken)) {
        return ERR_BAD_EXPRESSION;
    }
    Type = InputToken.Type;
    ID = InputToken.ID;
    Value = InputToken.Value;
    Arguments=ExprNodeVector;
    return NO_ERROR;
}


int ExpressionNode::InitialiseWithTokens (Token InputToken, std::vector<Token> tokenVector) {
    Type = InputToken.Type;
    ID = InputToken.ID;
    Value = InputToken.Value;
    Arguments.clear();
    for (int i=0; i<tokenVector.size(); i++) {
        if (tokenVector[i].Type==tFunction) {
            // check for function arguments
            int bracketCount=0;
            int argumentStart=0;
            int argumentEnd=0;
            if (tokenVector[i+1].Type==tBracket && tokenVector[i+1].ID==coOpenBracket) {
                bracketCount++;
                argumentStart=i+2;
                argumentEnd=argumentStart;
                while (bracketCount>0 && argumentEnd<tokenVector.size()) {
                    argumentEnd++;
                    if (tokenVector[argumentEnd].ID==coOpenBracket) {
                        bracketCount++;
                    } else if (tokenVector[argumentEnd].ID==coCloseBracket) {
                        bracketCount--;
                    }                    
                }
                if (bracketCount>0) {
                    return ERR_BAD_FUNCTION_BRACKET;
                }
                // arguments are in argumentStart to argumentEnd-1
                std::vector<Token> argumentTokens=std::vector<Token>(tokenVector.begin()+argumentStart, tokenVector.begin()+argumentEnd);
                std::vector<ExpressionNode> argumentNodes;
                int r=TokensToExpressionNodeCollection(argumentTokens, argumentNodes);
                if (r!=NO_ERROR) {
                    return r;
                }
                ExpressionNode node;
                int r2=node.InitialiseWithArguments(tokenVector[i], argumentNodes);
                if (r2!=NO_ERROR) {
                    return r;
                }
                Arguments.push_back(node);
                i=argumentEnd; // Here
            } else {
                // no arguments
                ExpressionNode node;
                int r=node.Initialise(tokenVector[i]);
                if (r!=NO_ERROR) {
                    return r;
                }
                Arguments.push_back(node);
            }
        } else {
            ExpressionNode node;
            int r=node.Initialise(tokenVector[i]);
            if (r!=NO_ERROR) {
                return r;
            }
            Arguments.push_back(node);
        }
    }
    return NO_ERROR;
}

std::string ExpressionNode::GetString() {
    std::string result;
    if (Type==tFunction) {
        result=Value+"(";
        if (Arguments.size()>0) {
            for (int i=0; i<Arguments.size(); i++) {
                if (i>0) {
                    result+=", ";
                }
                result+=Arguments[i].GetString();
            }
        }
        result+=")";
    } else if (Arguments.size()>0) {
        for (int i=0; i<Arguments.size(); i++) {
            if (i>0) {
                result+=" ";
            }
            result+=Arguments[i].GetString();
        }
    } else {
        result=Value;
    }
    return result;
}


ExpressionNode::~ExpressionNode() {
    Arguments.clear();
}  

int Expression::AddNode(ExpressionNode &ExprNode) {
    // add a command to the instruction
    // return command type
    nodes.push_back(ExprNode);
    return NO_ERROR;
}

int Expression::AddExpression(Expression &Expr) {
    // add a command to the instruction
    // return command type

    for (int i=0; i<Expr.nodes.size(); i++) {
        nodes.push_back(Expr.nodes[i]); 
    }
    return NO_ERROR;
}

int Expression::InitialiseWithTokens(const std::vector<Token> tokenVector) {

    nodes.clear();
    for (int i=0; i<tokenVector.size(); i++) {
        if (tokenVector[i].Type==tFunction) {
            // check for function arguments
            int bracketCount=0;
            int argumentStart=0;
            int argumentEnd=0;
            if (tokenVector[i+1].Type==tBracket && tokenVector[i+1].ID==coOpenBracket) {
                bracketCount++;
                argumentStart=i+2;
                argumentEnd=argumentStart;
                while (bracketCount>0 && argumentEnd<tokenVector.size()) {
                    argumentEnd++;
                    if (tokenVector[argumentEnd].ID==coOpenBracket) {
                        bracketCount++;
                    } else if (tokenVector[argumentEnd].ID==coCloseBracket) {
                        bracketCount--;
                    }                    
                }
                if (bracketCount>0) {
                    return ERR_BAD_FUNCTION_BRACKET;
                }
                // arguments are in argumentStart to argumentEnd-1
                std::vector<Token> argumentTokens=std::vector<Token>(tokenVector.begin()+argumentStart, tokenVector.begin()+argumentEnd);
                std::vector<ExpressionNode> argumentNodes;
                int r=TokensToExpressionNodeCollection(argumentTokens, argumentNodes);
                ExpressionNode node;
                int r2=node.InitialiseWithArguments(tokenVector[i], argumentNodes);
//                int r=node.InitialiseWithTokens(tokenVector[i], argumentTokens); // HERE
                if (r2!=NO_ERROR) {
                    return r;
                }
                nodes.push_back(node);
                i=argumentEnd;
            } else {
                // no arguments
                ExpressionNode node;
                int r=node.Initialise(tokenVector[i]);
                if (r!=NO_ERROR) {
                    return r;
                }
                nodes.push_back(node);
            }
        } else {
            ExpressionNode node;
            int r=node.Initialise(tokenVector[i]);
            if (r!=NO_ERROR) {
                return r;
            }
            nodes.push_back(node);
        }
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
        result+=nodes[i].GetString();
    }
    return result;
}