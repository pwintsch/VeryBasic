#include "command.hpp"
#include "error.hpp"
#include "token.hpp"
#include "syntax.hpp"
#include "console.hpp"
#include "processor.hpp"

#include <stdio.h>

typedef struct tVarValue {
	int iType;
	std::string sValue;
	float fValue;
} tVarValue;


bool IsElementOKforExpression(const int pType, const int pID) {
    switch (pType)
    {
    case tValue:
    case tString:
    case tUserDefined:
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


int CommandNode::Precedence() {
	switch (ID) {
		case coAND:
		case coOR:
		case coXOR:
			return 2;
			break;
		case coEqual:
			return 3;
			break;
		case coGreater:
		case coLess:
		case coGreaterEqual:
		case coGreaterLesser:
		case coLessEqual:
			return 4;
			break;
		case coPlus:
			return 5;
			break;
		case coMinus:
			return 6;
			break;
		case coMultiply:
			return 7;
			break;
		case coDivide:
			return 8;
			break;
		case coPower:
			return 9;
			break;
		case coUnaryMinus:
		case coNOT:
			return 10;
			break;
	}
	return 0;
}


int CommandNode::Evaluate(int &ResultType, float &NumResult, std::string &StrResult) {
std::vector<CommandNode> EvalStack;
std::vector<CommandNode> EvalQueue;
int NodePrecedence=0;
bool bPrintRPN=false;

    if (Type!=tExpression) {
        NumResult=0;
        StrResult="";
        return ERR_NOT_EXPRESSION;
    } else {
        // Convert to Reverse Polish Notation
        for (auto & element : SubArguments) {
            switch (element.Type) {
                case tValue:
                case tString:
                case tUserDefined:
                    EvalQueue.push_back(element);
                    break;
                case tFunction:
                case tUserFunction:
                    return ERR_NOT_AVAILABLE;
                    break;
                case tComparison:
                case tOperator:
                    if (EvalStack.size()>0 && EvalStack.back().Precedence()>element.Precedence()) {
                        EvalQueue.push_back(EvalStack.back());
                        EvalStack.pop_back();
                    }
                    EvalStack.push_back(element);
                    break;
                case tBracket:
                    if (element.ID==coOpenBracket) {
                        EvalStack.push_back(element);
                    } else if (element.ID==coCloseBracket) {
                        // replace [0] with top
                        while (EvalStack.size()>0 && EvalStack.back().ID!=coOpenBracket) {
                            EvalQueue.push_back(EvalStack.back());
                            EvalStack.pop_back();
                        }
                        if (EvalStack.size()==0) {
                            return ERR_BAD_BRACKET;
                        }
                        EvalStack.pop_back();
                    }
                    break;
                default:
                    return ERR_UNKNOWN_EXPRESSION_NODE;
            }
        }
        // while there are operators on the stack, pop them to the queue
        while (EvalStack.size()>0) {
            EvalQueue.push_back(EvalStack.back());
            EvalStack.pop_back();
        }
		// Generate Postfix notation
        std::string sRPN="Postfix : ";
		if (bPrintRPN) {
            int Qsize=EvalQueue.size();
			for (int i=0; i<Qsize; i++) {
				sRPN=sRPN+EvalQueue[i].Value;
                if (i<Qsize-1) sRPN=sRPN+", ";
			}
			Terminal.WriteLn(sRPN.c_str());
		}
        float Flt1;
        float Flt2;
        std::string Str1="";
        std::string Str2="";
        std::vector<tVarValue> Value;
        for (int i=0;i<EvalQueue.size();i++) {
            Flt1=0;
            Flt2=0;
            tVarValue TmpValue;
            TmpValue.iType = tUnknown;
            TmpValue.fValue = 0;
            TmpValue.sValue = "";
            tVarValue Value1;
            tVarValue Value2;
            bool ValueListWasEmpty=false;
            std::string VarStrValue="";
            int VarIntValue=0;
            float VarFltValue=0;
            int VarType=0;
            int r=NO_ERROR;
            switch (EvalQueue[i].Type) {
                case tValue:
                    TmpValue.iType = tValue;
                    TmpValue.fValue = std::stof(EvalQueue[i].Value);
                    Value.push_back(TmpValue);
                    break;
                case tString:
                    TmpValue.iType = tString;
                    TmpValue.sValue = EvalQueue[i].Value;
                    Value.push_back(TmpValue);
                    break;
                case tUserDefined:
                    r=MyProcessor.Variables.Get(EvalQueue[i].Value, VarType, VarFltValue, VarIntValue, VarStrValue);
                    if (r!=NO_ERROR) {
                        return r;
                    }
                    if (VarType==cvString) {
                        TmpValue.iType = tString;
                        TmpValue.sValue = VarStrValue;
                        Value.push_back(TmpValue);
                    } else {
                        TmpValue.iType = tValue;
                        if (VarType==cvInteger) {
                            TmpValue.fValue = (float)VarIntValue;
                        } else {
                            TmpValue.fValue = VarFltValue;
                        }
                        Value.push_back(TmpValue);
                    }
                    break;
                case tUserFunction:
                case tFunction:
                    return ERR_NOT_AVAILABLE;
                    break;
                case tOperator:
                    ValueListWasEmpty=true;
                    Value2 = Value.back();
                    Flt2 = Value2.fValue;
                    Value.pop_back();
                    if (Value.size() > 0) {
                        ValueListWasEmpty = false;
                        Value1 = Value.back();
                        Flt1 = Value1.fValue;
                        if (Value1.iType != Value2.iType) {
                            return ERR_MISMATCH_EXPRESSION_TYPES;
                        }
                        Value.pop_back();
                    }
                    if (Value2.iType == tValue) {
                        tVarValue NewValue;
                        NewValue.iType = tValue;
                        switch (EvalQueue[i].ID) {
                            case coAND:
                                NewValue.fValue = Flt1 && Flt2;
                                break;
                            case coOR:
                                NewValue.fValue = Flt1 || Flt2;
                                break;
                            case coPlus:
                                NewValue.fValue = Flt1 + Flt2;
                                break;
                            case coMinus:
                                NewValue.fValue = Flt1 - Flt2;
                                break;
                            case coMultiply:
                                NewValue.fValue = Flt1 * Flt2;
                                break;
                            case coDivide:
                                NewValue.fValue = Flt1 / Flt2;
                                break;
                            case coPower:
                                NewValue.fValue = pow(Flt1, Flt2);
                                break;
                            case coUnaryMinus:
                                if (! ValueListWasEmpty) Value.push_back(Value1);
                                NewValue.fValue = Flt2 * -1;
                                break;
                            case coNOT:
                                if (! ValueListWasEmpty) Value.push_back(Value1);
                                NewValue.fValue = (Flt2==0) ? 1 : 0;
                                break;
                            default:
                                return ERR_EXPRESSION_OPERATOR_DATATYPE;
                                break;
                        }
                        Value.push_back(NewValue);
                    } else if (Value2.iType == tString) {
                        tVarValue NewValue;
                        NewValue.iType = tString;
                        switch (EvalQueue[i].ID) {
                            case coPlus:
                                NewValue.sValue = Value1.sValue + Value2.sValue;
                                break;
                            default:
                                return ERR_EXPRESSION_OPERATOR_DATATYPE;
                                break;
                        }
                        Value.push_back(NewValue);
                    } else {
                        return ERR_UNKNOWN_EXPRESSION_DATA_TYPE;
                    }
                    break;
				case tComparison: {
                    float CompResult=0;
                    tVarValue NewValue;
                    NewValue.iType=tValue;
                    Value2=Value.back();
                    Value.pop_back();
                    Value1=Value.back();
                    Value.pop_back();

                    if (Value1.iType!=Value2.iType) {
                        return ERR_MISMATCH_EXPRESSION_TYPES;
                    }
                    switch (EvalQueue[i].ID) {
                        case coGreater:
                            if (Value2.iType==tValue) {
                                if (Value1.fValue>Value2.fValue) {
                                    CompResult=1;
                                }
                            } else if (Value1.sValue.compare(Value2.sValue)>0) {
                                CompResult=1;
                            }	
                            break; 
                        case coLess:
                            if (Value2.iType==tValue) {
                                if (Value1.fValue<Value2.fValue) {
                                    CompResult=1;
                                }
                            } else if (Value1.sValue.compare(Value2.sValue)<0) {
                                CompResult=1;
                            }	
                            break;
                        case coGreaterEqual:
                            if (Value2.iType==tValue) {
                                if (Value1.fValue>=Value2.fValue) {
                                    CompResult=1;
                                }
                            } else if (Value1.sValue.compare(Value2.sValue)>=0) {
                                CompResult=1;
                            }	
                            break;
                        case coLessEqual:
                            if (Value2.iType==tValue) {
                                if (Value1.fValue<=Value2.fValue) {
                                    CompResult=1;
                                }
                            } else if (Value1.sValue.compare(Value2.sValue)<=0) {
                                CompResult=1;
                            }
                            break;
                        case coEqual:
                            if (Value2.iType==tValue) {
                                if (Value1.fValue==Value2.fValue) {
                                    CompResult=1;
                                }
                            } else if (Value1.sValue.compare(Value2.sValue)==0) {
                                CompResult=1;
                            }
                            break;
                        case coGreaterLesser:
                            if (Value2.iType==tValue) {
                                if (Value1.fValue!=Value2.fValue) {
                                    CompResult=1;
                                }
                            } else if (Value1.sValue.compare(Value2.sValue)!=0) {
                                CompResult=1;
                            }
                            break; 
                        default:
                            return ERR_EXPRESSION_OPERATOR_DATATYPE;
                    }

                    NewValue.fValue=CompResult;
                    Value.push_back(NewValue); 
					break; 
                }
                default:
                    return ERR_UNKNOWN_EXPRESSION_NODE;
            }

        }
        ResultType=Value[0].iType;
        NumResult=Value[0].fValue;
        StrResult=Value[0].sValue;
    }
    return NO_ERROR;
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


std::string CommandNode::GetDetailedString(int Padding) {
    std::string s="";
    std::string p="";
    for (int i=0; i<Padding; i++) {
        p=p+" ";
    }
    s=p + GetSyntaxNodeString(Type) + " - " + GetTokenTextFromID(ID) + ", " + Value + " ";
    if (SubArguments.size()>0) {
        s=s+"has " + std::to_string(SubArguments.size()) +" sub-arguments: \n\r";
        for (int i=0; i<SubArguments.size(); i++) {
            s=s+p+"Sub-argument #" + std::to_string(i) +":\n\r";
            s=s+SubArguments[i].GetDetailedString(Padding+4);
        }
    } else {
        s=s+"\n\r";
    }
    return s;
} 



std::string CommandNode::GetString() {
    std::string s="";
    if (Type==tString) {
        s=s + "\"";
    }
    s=s + Value;
    if (Type==tString) {
        s=s + "\"";
    }
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
        if (tokenVector[i].Type==tFunction || (tokenVector[i].Type==tUserDefined && tokenVector[i+1].ID==coOpenBracket && i<tokenVector.size()-1)) {
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
            if (tokenVector[i].ID==coMinus) {
                if (i==0 || (tokenVector[i-1].Type!=tUserDefined && tokenVector[i-1].Type!=tValue && tokenVector[i-1].ID!=coCloseBracket)) {
                    // unary minus
                    tokenVector[i].ID=coUnaryMinus;
                }
            }
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
        if ((pTokens[i].Type==tFunction) || (pTokens[i].Type==tUserDefined && pTokens[i+1].ID==coOpenBracket && i<pTokens.size()-1)) {
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
    RuleNo=0;
    if (LexResults[0].Type == tUserDefined) {
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
                int ExpressionIndex=0;
                while (TokenIndex<Nodes.size() && IsCommandNodeOKForExpression(Nodes[TokenIndex])) {
                    if (Nodes[TokenIndex].ID==coMinus && (ExpressionIndex==0 || (Nodes[TokenIndex-1].Type!=tUserDefined && Nodes[TokenIndex-1].Type!=tValue && Nodes[TokenIndex-1].ID!=coCloseBracket))) {
                        // unary minus
                        Nodes[TokenIndex].ID=coUnaryMinus;
                    }
                    ExpressionCommandNodes.push_back(Nodes[TokenIndex]);
                    TokenIndex++;
                    ExpressionIndex++;
                }
                if (ExpressionCommandNodes.size()==0) {
                    RuleSearchError=true;
                }
                CommandNode Argument;
                Argument.InitialiseExpression(ExpressionCommandNodes);
                SyntaxIndex++;
                Arguments.push_back(Argument);
            } else if (SyntaxRules[i].Syntax[SyntaxIndex].iTType==tInputExpression) {
                CommandNode InputExpr;
                InputExpr.Type=tInputExpression;
                InputExpr.ID=0;
                InputExpr.Value="";
                std::vector<CommandNode> InputExpressionCommandNodes;
                while (TokenIndex<Nodes.size()) {
                    // the problem is that it is taking the close bracket into the expression might have to change order of things
                    // Txpically if openbracket start the expression including the open bracket so it closes off normally
                    if (Nodes[TokenIndex].ID==coComma || Nodes[TokenIndex].ID==coSemiColon || Nodes[TokenIndex].ID==coBackSlash || Nodes[TokenIndex].ID==coExclamation) {
                        CommandNode Argument;
                        Argument.InitialiseFromCommandNode(Nodes[TokenIndex]);
                        InputExpr.SubArguments.push_back(Argument);
                        TokenIndex++;
                    } else if (Nodes[TokenIndex].Type==tString || Nodes[TokenIndex].Type==tValue || Nodes[TokenIndex].ID==cvDouble || Nodes[TokenIndex].ID==cvSingle || Nodes[TokenIndex].ID==cvString || Nodes[TokenIndex].ID==cvInteger ) {
                        CommandNode Argument;
                        Argument.InitialiseFromCommandNode(Nodes[TokenIndex]);
                        InputExpr.SubArguments.push_back(Argument);
                        TokenIndex++;                          
                    } else if (Nodes[TokenIndex].ID==coOpenBracket) {
                        int ExpressionIndex=0;
                        while (IsCommandNodeOKForExpression(Nodes[TokenIndex])) {
                            if (Nodes[TokenIndex].ID==coMinus && (ExpressionIndex==0 || (Nodes[TokenIndex-1].Type!=tUserDefined && Nodes[TokenIndex-1].Type!=tValue && Nodes[TokenIndex-1].ID!=coCloseBracket))) {
                                // unary minus
                                Nodes[TokenIndex].ID=coUnaryMinus;
                            }
                            InputExpressionCommandNodes.push_back(Nodes[TokenIndex]);
                            TokenIndex++;
                            ExpressionIndex++;
                        }
                        if (InputExpressionCommandNodes.size()!=0) {
                            CommandNode Argument;
                            Argument.InitialiseExpression(InputExpressionCommandNodes);
                            InputExpr.SubArguments.push_back(Argument);
                            InputExpressionCommandNodes.clear();
                        } else {
                            return ERR_BAD_INPUT_EXPRESSION;
                        }
                    } else if (Nodes[TokenIndex].ID==coTAB) {
                        CommandNode TabNode;
                        TabNode.InitialiseFromCommandNode (Nodes[TokenIndex]);
                        TokenIndex++;
                        std::vector<CommandNode> TabArgument;
                        int ExpressionIndex=0;
                        while (IsCommandNodeOKForExpression(Nodes[TokenIndex])) {
                            if (Nodes[TokenIndex].ID==coMinus && (ExpressionIndex==0 || (Nodes[TokenIndex-1].Type!=tUserDefined && Nodes[TokenIndex-1].Type!=tValue && Nodes[TokenIndex-1].ID!=coCloseBracket))) {
                                // unary minus
                                Nodes[TokenIndex].ID=coUnaryMinus;
                            }
                            TabArgument.push_back(Nodes[TokenIndex]);
                            TokenIndex++;
                            ExpressionIndex++;
                        }
                        if (TabArgument.size()!=0) {
                            CommandNode Argument;
                            Argument.InitialiseExpression(TabArgument);
                            TabNode.SubArguments.push_back(Argument);
                            InputExpr.SubArguments.push_back(TabNode);
                            InputExpressionCommandNodes.clear();
                        } else {
                            return ERR_BAD_PRINT_EXPRESSION;
                        }
                    } else if (Nodes[TokenIndex].ID==coAT) {
                        CommandNode AtNode;
                        AtNode.InitialiseFromCommandNode (Nodes[TokenIndex]);
                        TokenIndex++;
                        std::vector<CommandNode> AtArgument;
                        int ExpressionIndex=0;
                        while (IsCommandNodeOKForExpression(Nodes[TokenIndex])) {
                            if (Nodes[TokenIndex].ID==coMinus && (ExpressionIndex==0 || (Nodes[TokenIndex-1].Type!=tUserDefined && Nodes[TokenIndex-1].Type!=tValue && Nodes[TokenIndex-1].ID!=coCloseBracket))) {
                                // unary minus
                                Nodes[TokenIndex].ID=coUnaryMinus;
                            }
                            AtArgument.push_back(Nodes[TokenIndex]);
                            TokenIndex++;
                            ExpressionIndex++;
                        }
                        if (AtArgument.size()!=0) {
                            CommandNode Argument;
                            Argument.InitialiseExpression(AtArgument);
                            AtNode.SubArguments.push_back(Argument);
                            AtArgument.clear();
                        } else {
                            return ERR_BAD_PRINT_EXPRESSION;
                        }
                        if (Nodes[TokenIndex].ID==coComma) {
                            CommandNode Argument;
                            Argument.InitialiseFromCommandNode(Nodes[TokenIndex]);
                            AtNode.SubArguments.push_back(Argument);
                            TokenIndex++;
                        } else {
                            return ERR_BAD_PRINT_EXPRESSION;
                        }
                        ExpressionIndex=0;
                        while (IsCommandNodeOKForExpression(Nodes[TokenIndex])) {
                            if (Nodes[TokenIndex].ID==coMinus && (ExpressionIndex==0 || (Nodes[TokenIndex-1].Type!=tUserDefined && Nodes[TokenIndex-1].Type!=tValue && Nodes[TokenIndex-1].ID!=coCloseBracket))) {
                                // unary minus
                                Nodes[TokenIndex].ID=coUnaryMinus;
                            }
                            AtArgument.push_back(Nodes[TokenIndex]);
                            TokenIndex++;
                            ExpressionIndex++;
                        }
                        if (AtArgument.size()!=0) {
                            CommandNode Argument;
                            Argument.InitialiseExpression(AtArgument);
                            AtNode.SubArguments.push_back(Argument);
                            InputExpr.SubArguments.push_back(AtNode);
                            InputExpressionCommandNodes.clear();
                        } else {
                            return ERR_BAD_PRINT_EXPRESSION;
                        }
                    } else if (TokenIndex<Nodes.size()) {
                        RuleSearchError=true;
                    }
                } 
                if (InputExpr.SubArguments.size()==0) { 
                    return ERR_BAD_INPUT_EXPRESSION;               
                } else { 
                    Arguments.push_back(InputExpr);
                    SyntaxIndex++;
                }
            } else if (SyntaxRules[i].Syntax[SyntaxIndex].iTType==tPrintExpression) {
                CommandNode PrintExpr;
                PrintExpr.Type=tPrintExpression;
                PrintExpr.ID=0;
                PrintExpr.Value="";
                std::vector<CommandNode> PrintExpressionCommandNodes;
                while (TokenIndex<Nodes.size()) {
                    // adapt for if below to become a while function (while token for expression make expression if not and control char then add token for control char and continue till end of line)
                    int ExpressionIndex=0;
                    while (IsCommandNodeOKForExpression(Nodes[TokenIndex])) {
                        if (Nodes[TokenIndex].ID==coMinus && (ExpressionIndex==0 || (Nodes[TokenIndex-1].Type!=tUserDefined && Nodes[TokenIndex-1].Type!=tValue && Nodes[TokenIndex-1].ID!=coCloseBracket))) {
                            // unary minus
                            Nodes[TokenIndex].ID=coUnaryMinus;
                        }
                        PrintExpressionCommandNodes.push_back(Nodes[TokenIndex]);
                        TokenIndex++;
                        ExpressionIndex++;
                    } 
                    if (PrintExpressionCommandNodes.size()!=0) {
                        CommandNode Argument;
                        Argument.InitialiseExpression(PrintExpressionCommandNodes);
                        PrintExpr.SubArguments.push_back(Argument);
                        PrintExpressionCommandNodes.clear();
                    }
                    if (Nodes[TokenIndex].ID==coComma || Nodes[TokenIndex].ID==coSemiColon || Nodes[TokenIndex].ID==coBackSlash || Nodes[TokenIndex].ID==coExclamation) {
                        CommandNode Argument;
                        Argument.InitialiseFromCommandNode(Nodes[TokenIndex]);
                        PrintExpr.SubArguments.push_back(Argument);
                        TokenIndex++;
                    } else if (Nodes[TokenIndex].ID==coTAB) {
                        CommandNode TabNode;
                        TabNode.InitialiseFromCommandNode (Nodes[TokenIndex]);
                        TokenIndex++;
                        std::vector<CommandNode> TabArgument;
                        int ExpressionIndex=0;
                        while (IsCommandNodeOKForExpression(Nodes[TokenIndex])) {
                            if (Nodes[TokenIndex].ID==coMinus && (ExpressionIndex==0 || (Nodes[TokenIndex-1].Type!=tUserDefined && Nodes[TokenIndex-1].Type!=tValue && Nodes[TokenIndex-1].ID!=coCloseBracket))) {
                                // unary minus
                                Nodes[TokenIndex].ID=coUnaryMinus;
                            }
                            TabArgument.push_back(Nodes[TokenIndex]);
                            TokenIndex++;
                            ExpressionIndex++;
                        }
                        if (TabArgument.size()!=0) {
                            CommandNode Argument;
                            Argument.InitialiseExpression(TabArgument);
                            TabNode.SubArguments.push_back(Argument);
                            PrintExpr.SubArguments.push_back(TabNode);
                            PrintExpressionCommandNodes.clear();
                        } else {
                            return ERR_BAD_PRINT_EXPRESSION;
                        }
                    } else if (Nodes[TokenIndex].ID==coAT) {
                        CommandNode AtNode;
                        AtNode.InitialiseFromCommandNode (Nodes[TokenIndex]);
                        TokenIndex++;
                        std::vector<CommandNode> AtArgument;
                        int ExpressionIndex=0;
                        while (IsCommandNodeOKForExpression(Nodes[TokenIndex])) {
                            if (Nodes[TokenIndex].ID==coMinus && (ExpressionIndex==0 || (Nodes[TokenIndex-1].Type!=tUserDefined && Nodes[TokenIndex-1].Type!=tValue && Nodes[TokenIndex-1].ID!=coCloseBracket))) {
                                // unary minus
                                Nodes[TokenIndex].ID=coUnaryMinus;
                            }
                            AtArgument.push_back(Nodes[TokenIndex]);
                            TokenIndex++;
                            ExpressionIndex++;
                        }
                        if (AtArgument.size()!=0) {
                            CommandNode Argument;
                            Argument.InitialiseExpression(AtArgument);
                            AtNode.SubArguments.push_back(Argument);
                            AtArgument.clear();
                        } else {
                            return ERR_BAD_PRINT_EXPRESSION;
                        }
                        if (Nodes[TokenIndex].ID==coComma) {
                            CommandNode Argument;
                            Argument.InitialiseFromCommandNode(Nodes[TokenIndex]);
                            AtNode.SubArguments.push_back(Argument);
                            TokenIndex++;
                        } else {
                            return ERR_BAD_PRINT_EXPRESSION;
                        }
                        ExpressionIndex=0;
                        while (IsCommandNodeOKForExpression(Nodes[TokenIndex])) {
                            if (Nodes[TokenIndex].ID==coMinus && (ExpressionIndex==0 || (Nodes[TokenIndex-1].Type!=tUserDefined && Nodes[TokenIndex-1].Type!=tValue && Nodes[TokenIndex-1].ID!=coCloseBracket))) {
                                // unary minus
                                Nodes[TokenIndex].ID=coUnaryMinus;
                            }
                            AtArgument.push_back(Nodes[TokenIndex]);
                            TokenIndex++;
                            ExpressionIndex++;
                        }
                        if (AtArgument.size()!=0) {
                            CommandNode Argument;
                            Argument.InitialiseExpression(AtArgument);
                            AtNode.SubArguments.push_back(Argument);
                            PrintExpr.SubArguments.push_back(AtNode);
                            PrintExpressionCommandNodes.clear();
                        } else {
                            return ERR_BAD_PRINT_EXPRESSION;
                        }
                    } else if (TokenIndex<Nodes.size()) {
                        RuleSearchError=true;
                    }
                }
                Arguments.push_back(PrintExpr);
                SyntaxIndex++;
            } else {
                break;
            }
        }
        if (RuleSearchError==false && SyntaxIndex==SyntaxRules[i].Syntax.size() && TokenIndex>=Nodes.size()) {
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
    if (s!="") {
        s=s+" ";
    } 
    for (int i=0; i<Arguments.size(); i++) {
        if (i>0) {
            s=s+" ";
        }
        s=s+Arguments[i].GetString();
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
    s="Command: " + std::to_string(ID) + " - " + std::to_string(Type) + " :  " + GetTokenTextFromID(ID) + "\n\r";
    s=s+"Rule#: " + std::to_string(RuleNo) + "\n\r";
    s=s+"Total of " + std::to_string(Arguments.size())+ " arguments: \n\r";
    for (int i=0; i<Arguments.size(); i++) {
        s=s+"Argument #" + std::to_string(i) +":\n\r";
        s=s+Arguments[i].GetDetailedString(4);
    }
    s=s+"\n\r";
    return s;
}