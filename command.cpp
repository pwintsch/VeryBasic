#include "command.hpp"
#include "error.hpp"
#include "token.hpp"
#include "syntax.hpp"
#include <stdio.h>


 bool IsCommandNodeOKForExpression(const CommandNode &CmdNode) {

    return IsElementOKforExpression(CmdNode.Type, CmdNode.ID);
}



CommandNode::CommandNode() {
    // TODO Auto-generated constructor stub
    ID=0;
    Type=0;
    CmdExpr=Expression();
}

CommandNode::~CommandNode() {
    // TODO Auto-generated destructor stub
}

int CommandNode::InitialiseFromCommandNode(CommandNode &SourceNode) {
    ID=SourceNode.ID;
    Type=SourceNode.Type;
    Value=SourceNode.Value;
    CmdExpr=SourceNode.CmdExpr;
    SubArguments=SourceNode.SubArguments;
    return NO_ERROR;
}

int CommandNode::InitialiseFromToken (Token &SourceToken) {
    ID=SourceToken.ID;
    Type=SourceToken.Type;
    Value=SourceToken.Value;
    SubArguments.clear();
    return NO_ERROR;
}

int CommandNode::InitialiseWithArguments(const Token &SourceToken, std::vector<Expression> &pArguments) {
    ID=SourceToken.ID;
    Type=SourceToken.Type;
    Value=SourceToken.Value;
    SubArguments=pArguments;
    return NO_ERROR;
}

int CommandNode::InitialiseExpression(int pType, int pID, Expression &pExpression) {
    ID=pID;
    Type=pType;
    Value="";
    CmdExpr=pExpression;
    SubArguments.clear();
    return NO_ERROR;
 }


Command::~Command() {
    // TODO Auto-generated destructor stub
    Arguments.clear();
}


int Command::Lexerize(std::vector<Token> &pTokens) {
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
            std::vector<Expression> SubArgs;
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


int Command::FindSyntaxRule() {
    int RuleKey=LexResults[0].ID;
    Type=LexResults[0].Type;
    ID=LexResults[0].ID;
    int RuleNo=0;
    if (LexResults[0].Type == tVariable) {
        RuleKey=coVariable;
    }
    std::vector<tSyntax> SyntaxRules=GetSyntaxRules(RuleKey);
    Rules=SyntaxRules;
    if (SyntaxRules.size()==0) {
        return ERR_BAD_COMMAND;
    }
    bool RuleFound=false;
    
    for (int i=0; i<SyntaxRules.size(); i++) {
        std::vector<CommandNode> Nodes=LexResults;
        if (Tokens.size()==1 && SyntaxRules[i].Syntax.size()==1) {
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
                Expression exp;
                // move command nodes to expression as ExpressionNodes
                for (int j=0; j<ExpressionCommandNodes.size(); j++) {
                    ExpressionNode ExprNode;
                    if (ExpressionCommandNodes[j].Type!=tExpression) {
                        /// if ExpressionCommandNodes[j] is a function need to pass sub arguments to new ExpressionNode
                        ExprNode.BasicInitialise(ExpressionCommandNodes[j].Type, ExpressionCommandNodes[j].ID, ExpressionCommandNodes[j].Value);
                        if (ExpressionCommandNodes[j].SubArguments.size()>0) {
                            for (int k=0; k<ExpressionCommandNodes[j].SubArguments.size(); k++) {
                                // Add an Argument (ExpressionNode) for each subArgument (Expression)
                                // create an ExpressionNode for each subArgument and add to ExprNode.Arguments
                                ExpressionNode ArgExp;
                                ArgExp.BasicInitialise(tExpression, 0, "");
                                ArgExp.Arguments=ExpressionCommandNodes[j].SubArguments[k].nodes;   
                                ExprNode.Arguments.push_back(ArgExp);;
                            }
                        }
                        exp.AddNode(ExprNode);
                    } else {
                        exp.AddExpression (ExpressionCommandNodes[j].CmdExpr);
                    }
                }

//                if (r!=NO_ERROR) {
//                    RuleSearchError=true;
//                }
                CommandNode Argument;
                Argument.InitialiseExpression(SyntaxRules[i].Syntax[SyntaxIndex].iTType, SyntaxRules[i].Syntax[SyntaxIndex].iTId, exp);
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

    int r=Lexerize(pTokens);
    if (r!=NO_ERROR) {
        return r;
    }

    Arguments.clear();


    r=FindSyntaxRule();
    if (r!=NO_ERROR) {
        return r;
    }
    return NO_ERROR;

}


// Returns Bad Command if no syntax rules found and if not type of command tDirectCommand, tCommand or tVariable

int Command::OldInitialise(std::vector<Token> &pTokens) {
    int RuleKey=pTokens[0].ID;
    RuleNo=0;
    Tokens=pTokens;
    Type=pTokens[0].Type;
    ID=pTokens[0].ID;
    if (pTokens[0].Type == tVariable) {
        RuleKey=coVariable;
    }
    std::vector<tSyntax> SyntaxRules=GetSyntaxRules(RuleKey);
    Rules=SyntaxRules;
    if (SyntaxRules.size()==0) {
        return ERR_BAD_COMMAND;
    }

   bool RuleFound=false;
    for (int i=0; i<SyntaxRules.size(); i++) {
        std::vector<Token> Tokens=pTokens;
        if (Tokens.size()==1 && SyntaxRules[i].Syntax.size()==1) {
            RuleFound=true;
            RuleNo=SyntaxRules[i].iRuleNo;
            Arguments.clear();
            return NO_ERROR;
        } 
        int TokenIndex=1;
        int SyntaxIndex=1;
        bool RuleSearchError=false;
        while (RuleFound==false && TokenIndex<Tokens.size() && SyntaxIndex<SyntaxRules[i].Syntax.size() && RuleSearchError==false) {
            
           // If next token is a variable then cjeck its not an array (next token is a bracket) and if so look for next token that is a close bracket or a comma (multiple dimension) and create an expression node for each dimension
           // if next token is a function and then do as with array

            // but initally just create a matching node if apt
      
            if (Tokens[TokenIndex].Type==SyntaxRules[i].Syntax[SyntaxIndex].iTType) {
                CommandNode Argument;
                Argument.InitialiseFromToken(Tokens[TokenIndex]);
                Arguments.push_back(Argument);
                TokenIndex++;
                SyntaxIndex++;
            } else if (SyntaxRules[i].Syntax[SyntaxIndex].iTType==tExpression) {
                std::vector<Token> ExpressionTokens;
                while (TokenIndex<Tokens.size() && IsTokenOKForExpression(Tokens[TokenIndex])) {
                    ExpressionTokens.push_back(Tokens[TokenIndex]);
                    TokenIndex++;
                }
                if (ExpressionTokens.size()==0) {
                    RuleSearchError=true;
                }
                Expression exp;
                int r=exp.InitialiseWithTokens(ExpressionTokens);
                if (r!=NO_ERROR) {
                    RuleSearchError=true;
                }
                CommandNode Argument;
                Argument.InitialiseExpression(SyntaxRules[i].Syntax[SyntaxIndex].iTType, SyntaxRules[i].Syntax[SyntaxIndex].iTId, exp);
                SyntaxIndex++;
                Arguments.push_back(Argument);
            } else {
                break;
            }
        }
        if (RuleSearchError==false && SyntaxIndex==SyntaxRules[i].Syntax.size() && TokenIndex==Tokens.size()) {
            RuleFound=true;
            RuleNo=SyntaxRules[i].iRuleNo;
            return NO_ERROR;
        }
        Arguments.clear();
 //       int r=InitialiseCommandNode(pTokens, SyntaxRules[i]);
 //       if (r==NO_ERROR) {
 //           return NO_ERROR;
 //       }
    }
    if (RuleFound==false) {
        return ERR_NO_MATCHING_SYNTAX;
    }
    return NO_ERROR;
}

std::string Command::GetString() {
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
            s=s + "Expression: " + Arguments[i].CmdExpr.GetString();
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