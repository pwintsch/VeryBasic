#include "command.hpp"
#include "error.hpp"
#include "token.hpp"
#include "syntax.hpp"
#include "expression.hpp"
#include <stdio.h>

CommandNode::CommandNode() {
    // TODO Auto-generated constructor stub
    ID=0;
    Type=0;
}

CommandNode::~CommandNode() {
    // TODO Auto-generated destructor stub
}

int CommandNode::Initialise (Token &SourceToken) {
    ID=SourceToken.ID;
    Type=SourceToken.Type;
    Value=SourceToken.Value;
    return NO_ERROR;
}



Command::~Command() {
    // TODO Auto-generated destructor stub
    Arguments.clear();
}

// Returns Bad Command if no syntax rules found and if not type of command tDirectCommand, tCommand or tVariable

int Command::Initialise(std::vector<Token> &pTokens) {
    int RuleKey=pTokens[0].ID;
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
            Arguments.clear();
            return NO_ERROR;
        } 
        int TokenIndex=1;
        for (int j=1; j<SyntaxRules[i].Syntax.size(); j++) {
            
           // If next token is a variable then cjeck its not an array (next token is a bracket) and if so look for next token that is a close bracket or a comma (multiple dimension) and create an expression node for each dimension
           // if next token is a function and then do as with array

            // but initally just create a matching node if apt
      
            if (Tokens[TokenIndex].Type==SyntaxRules[i].Syntax[j].iTType) {
                CommandNode Argument;
                Argument.Initialise(Tokens[TokenIndex]);

            }
        }
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
    for (int i=0; i<Tokens.size(); i++) {
        if (i>0) {
            s=s + ", ";
        }
        s=s + " - {" + std::to_string(Tokens[i].Type) + ":" + std::to_string(Tokens[i].ID) + ":" + Tokens[i].Value + "} ";
    }
    s=s+"\n\r";
    std::vector<tSyntax> SyntaxRules=Rules;
    if (SyntaxRules.size()==0) {
        s=s+"No Syntax Rules\n\r";
    } else {
        for (int i=0; i<SyntaxRules.size(); i++) {
            s=s+"Rule#: " + std::to_string(SyntaxRules[i].iRuleNo) + " - ";
            s=s+"Syntax: " + StringFromSyntaxRule ((SyntaxRules[i].Syntax));
            s=s+"\n\r";
        }
    }
    s=s+"\n\r";
    return s;
}