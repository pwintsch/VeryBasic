#include "command.hpp"
#include "error.hpp"
#include "token.hpp"
#include "syntax.hpp"

#include <stdio.h>


bool IsElementOKforExpression(const int pType, const int pID) {
    switch (pType)
    {
    case tValue:
    case tVariable:
    case tFunction:
    case tOperator:
    case tComparison:
        return true;
        break;
    case tBracket:
        if (pID == coOpenBracket || pID == coCloseBracket) {
            return true;
        } 
        break;
    
    default:
        break;
    }

    return false;
}


bool IsTokenOKForExpression(const Token &token) {

    return IsElementOKforExpression(token.Type, token.ID) ;
}


 bool IsCommandNodeOKForExpression(const CommandNode &CmdNode) {

    return IsElementOKforExpression(CmdNode.Type, CmdNode.ID);
}



int TokensToExpressionCollection( std::vector<Token> pTokens, std::vector<CommandNode> &pLexResults) {
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
            CommandNode Expr;
            Expr.Type=tExpression;
            int r=Expr.InitialiseAsExpressionWithTokens(std::vector<Token>(pTokens.begin() + LastStart, pTokens.begin() + i));
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
        CommandNode Expr;
        Expr.Type=tExpression;
        int r=Expr.InitialiseAsExpressionWithTokens(std::vector<Token>(pTokens.begin() + LastStart, pTokens.begin() + i));
        if (r!=NO_ERROR) {
            return r;
        }
        pLexResults.push_back(Expr);
    }
   return NO_ERROR; 
}




CommandNode::CommandNode() {
    // TODO Auto-generated constructor stub
    ID=0;
    Type=0;
}


CommandNode::~CommandNode() {

    // TODO Auto-generated destructor stub
}


int CommandNode::InitialiseFromCommandNode(CommandNode &SourceNode) {
    ID=SourceNode.ID;
    Type=SourceNode.Type;
    Value=SourceNode.Value;
    SubArguments=SourceNode.SubArguments;
    return NO_ERROR;
}

int CommandNode::BasicInitialise (int pType, int pID, std::string pValue) {
    Type=pType;
    ID=pID;
    Value=pValue;
    return NO_ERROR;
}

int CommandNode::InitialiseFromToken (Token &SourceToken) {
    ID=SourceToken.ID;
    Type=SourceToken.Type;
    Value=SourceToken.Value;
    SubArguments.clear();
    return NO_ERROR;
}

int CommandNode::InitialiseWithArguments(const Token &SourceToken, std::vector<CommandNode> &pArguments) {
    ID=SourceToken.ID;
    Type=SourceToken.Type;
    Value=SourceToken.Value;
    SubArguments=pArguments;
    return NO_ERROR;
}

int CommandNode::InitialiseExpression(std::vector<CommandNode> &pArguments) {
    ID=0;
    Type=tExpression;
    Value="";
    SubArguments=pArguments;
    return NO_ERROR;
 }



std::string CommandNode::GetString() {
    std::string s="";
    s=s + Value;
    if (SubArguments.size()>0) {
        if (Type==tFunction) s=s+"(";
        for (int j=0; j<SubArguments.size(); j++) {
            if (j>0 && Type==tFunction) {
                s=s + ", ";
            }
            s=s + SubArguments[j].GetString();
        }
        if (Type==tFunction) s=s+")";
    }
    return s;
}



int CommandNode::NodeCount() {
    int i=1;
    for (int j=0; j<SubArguments.size(); j++) {
        i=i+SubArguments[j].NodeCount();
    }
    return i;
};



int CommandNode::InitialiseAsExpressionWithTokens (std::vector<Token> tokenVector) {
    Type = tExpression;
    ID = 0;
    Value = "";
    SubArguments.clear();
    for (int i=0; i<tokenVector.size(); i++) {
        if (tokenVector[i].Type==tFunction || (tokenVector[i].Type==tVariable && tokenVector[i+1].ID==coOpenBracket && i<tokenVector.size()-1)) {
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
                std::vector<CommandNode> argumentNodes;
                int r=TokensToExpressionCollection(argumentTokens, argumentNodes);
                if (r!=NO_ERROR) {
                    return r;
                }
                CommandNode node;
                int r2=node.InitialiseWithArguments(tokenVector[i], argumentNodes);
                if (r2!=NO_ERROR) {
                    return r;
                }
                SubArguments.push_back(node);
                i=argumentEnd; // Here
            } else {
                // no arguments
                CommandNode node;
                int r=node.InitialiseFromToken(tokenVector[i]);
                if (r!=NO_ERROR) {
                    return r;
                }
                SubArguments.push_back(node);
            }
        } else {
            CommandNode node;
            int r=node.InitialiseFromToken(tokenVector[i]);
            if (r!=NO_ERROR) {
                return r;
            }
            SubArguments.push_back(node);
        }
    }
    return NO_ERROR;
}




Command::~Command() {
    // TODO Auto-generated destructor stub
    Arguments.clear();
}



int Command::Lexerize(std::vector<Token> &pTokens, std::vector<CommandNode> &LexResults) {
// Lexer takes Tokens and creates Command Nodes and builds Array Items and Function Call Nodes with expressions for parametres/indices
// Results put into command's LexResults Vector of CommandNodes
    LexResults.clear();
    for (int i=0; i<pTokens.size(); i++) {
        int j=0;
        if ((pTokens[i].Type==tFunction) || (pTokens[i].Type==tVariable && pTokens[i+1].ID==coOpenBracket && i<pTokens.size()-1)) {
            // Capture parametres within brackets and create expression node
            // Check to see if next token is an open bracket
            int BracketCount=0;
            if (pTokens[i+1].ID==coOpenBracket) {
                // find matching close bracket
                BracketCount=1;
                j=i+2;
                while (j<pTokens.size() && BracketCount>0) {
                    if (pTokens[j].ID==coOpenBracket) {
                        BracketCount++;
                    } else if (pTokens[j].ID==coCloseBracket) {
                        BracketCount--;
                    }
                    j++;
                }
            } else {
                BracketCount=-1;
            }
            if (BracketCount!=0) {
                return ERR_BAD_FUNCTION_BRACKET;
            } 
            std::vector<Token> SubTokens(pTokens.begin()+i+2, pTokens.begin()+j-1);
//            std::vector<Expression> SubArgs;
            std::vector<CommandNode> SubArgs;
            int r=TokensToExpressionCollection(SubTokens, SubArgs);
            if (r!=NO_ERROR) {
                return r;
            }
            CommandNode NewNode;
            NewNode.InitialiseWithArguments(pTokens[i], SubArgs);
            LexResults.push_back(NewNode);
            i=j-1;
            
        } else {
            // define newnode copy content of token
            CommandNode NewNode;
            NewNode.InitialiseFromToken(pTokens[i]);
            LexResults.push_back(NewNode);
        }
    }
    return NO_ERROR;
}


int Command::FindSyntaxRule(std::vector<CommandNode> &LexResults) {
// Find the syntax rule that matches the command nodes

    int RuleKey=LexResults[0].ID;
    Type=LexResults[0].Type;
    ID=LexResults[0].ID;
    int RuleNo=0;
    if (LexResults[0].Type == tVariable) {
        RuleKey=coVariable;
    }
    std::vector<tSyntax> SyntaxRules=GetSyntaxRules(RuleKey);
//    Rules=SyntaxRules;
    if (SyntaxRules.size()==0) {
        return ERR_BAD_COMMAND;
    }
    bool RuleFound=false;
    
    for (int i=0; i<SyntaxRules.size(); i++) {
        std::vector<CommandNode> Nodes=LexResults;
        if (LexResults.size()==1 && SyntaxRules[i].Syntax.size()==1) {
            RuleFound=true;
            RuleNo=SyntaxRules[i].iRuleNo;
            Arguments.clear();
            return NO_ERROR;
        } 
        int TokenIndex=1;
        int SyntaxIndex=1;
        if (RuleKey==coVariable) {
            TokenIndex=0;
            SyntaxIndex=0;
        }
        bool RuleSearchError=false;
        while (RuleFound==false && TokenIndex<Nodes.size() && SyntaxIndex<SyntaxRules[i].Syntax.size() && RuleSearchError==false) {     
            if (Nodes[TokenIndex].Type==SyntaxRules[i].Syntax[SyntaxIndex].iTType) {
                CommandNode Argument;
                Argument.InitialiseFromCommandNode(Nodes[TokenIndex]);
                Arguments.push_back(Argument);
                TokenIndex++;
                SyntaxIndex++;
            } else if (SyntaxRules[i].Syntax[SyntaxIndex].iTType==tExpression) {
// build Expression CommanNode from following CommandNodes that are allowed in an expression
                std::vector<CommandNode> ExpressionCommandNodes;
                while (TokenIndex<Nodes.size() && IsCommandNodeOKForExpression(Nodes[TokenIndex])) {
                    ExpressionCommandNodes.push_back(Nodes[TokenIndex]);
                    TokenIndex++;
                }
                if (ExpressionCommandNodes.size()==0) {
                    RuleSearchError=true;
                }
                CommandNode Argument;
                Argument.InitialiseExpression(ExpressionCommandNodes);
                SyntaxIndex++;
                Arguments.push_back(Argument);
            } else {
                break;
            }
        }
        if (RuleSearchError==false && SyntaxIndex==SyntaxRules[i].Syntax.size() && TokenIndex==Nodes.size()) {
            RuleFound=true;
            RuleNo=SyntaxRules[i].iRuleNo;
            return NO_ERROR;
        }
        Arguments.clear();        
    }
    if (RuleFound==false) {
        return ERR_NO_MATCHING_SYNTAX;
    } 
    return NO_ERROR;

}


int Command::Initialise(std::vector<Token> &pTokens) {
// First step transform Tokens into Command Nodes using lexer

    std::vector<CommandNode> TmpNodes;

    int r=Lexerize(pTokens, TmpNodes);
    if (r!=NO_ERROR) {
        return r;
    }

    Arguments.clear();



    r=FindSyntaxRule(TmpNodes);
    if (r!=NO_ERROR) {
        return r;
    }
    return NO_ERROR;

}


std::string Command::GetString() {
    std::string s="";
    s=GetTokenTextFromID(ID);
    for (int i=0; i<Arguments.size(); i++) {
        s=s+ " " +Arguments[i].GetString();
    }
    return s;
}


int Command::NodeCount() {
    int i=1;
    for (int j=0; j<Arguments.size(); j++) {
        i=i+Arguments[j].NodeCount();
    }
    return i;
};


std::string Command::GetDetailedString() {
    std::string s="";
    s="Command: " + std::to_string(ID) + " - " + std::to_string(Type) + " :  ";
    for (int i=0; i<Arguments.size(); i++) {
        if (i>0) {
            s=s + ", ";
        }
        s=s + " - {" + std::to_string(Arguments[i].Type) + ":" + std::to_string(Arguments[i].ID) + ":" + Arguments[i].Value + "} ";
        if (Arguments[i].Type==tVariable && Arguments[i].SubArguments.size()>0) {
            s=s + "Array Item with  " +std::to_string(Arguments[i].SubArguments.size()) + " arguments" ; //+ Arguments[i].ExpressionNode.GetString();
            s=s+"\n\r";
            if (Arguments[i].SubArguments.size()>0) {
                s=s+"Arguments: \n\r";
                for (int j=0; j<Arguments[i].SubArguments.size(); j++) {
                    s=s+"Argument " + std::to_string(j) + ": " + Arguments[i].SubArguments[j].GetString();
                    s=s+"\n\r";
                }
            }
        }
        if (Arguments[i].Type==tExpression) {
            s=s + "Expression: " + Arguments[i].GetString();
        }
        if (Arguments[i].Type==tFunction) {
            s=s + "Function with " +std::to_string(Arguments[i].SubArguments.size()) + " arguments" ; //+ Arguments[i].ExpressionNode.GetString();
            s=s+"\n\r";
            if (Arguments[i].SubArguments.size()>0) {
                s=s+"Arguments: \n\r";
                for (int j=0; j<Arguments[i].SubArguments.size(); j++) {
                    s=s+"Argument " + std::to_string(j) + ": " + Arguments[i].SubArguments[j].GetString();
                    s=s+"\n\r";
                }
            }
        }
    }
    s=s+"\n\r";
    s=s+"Rule#: " + std::to_string(RuleNo);
    s=s+"\n\r";
    return s;
}