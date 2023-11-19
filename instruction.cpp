#include "instruction.hpp"
#include "syntax.hpp"
#include "error.hpp"
#include <string>


int Instruction::Initialise(TokenCollection &InputTokens) {
    int iPosition=0;
    // if first token is a number, then it's a program line
    if (InputTokens.Tokens[0].ID == coInteger) {
        ProgramLine = atoi(InputTokens.Tokens[0].Value.c_str());
        if (InputTokens.Tokens.size()==1) {
            return NO_ERROR;
        }
        iPosition=1;
    } else {
        ProgramLine = 0;
    }
    int i;
    // look for colons and split accordingly for each command
    for (i=iPosition; i<InputTokens.Tokens.size(); i++){
        if (InputTokens.Tokens[i].ID == coColon && i>iPosition) {   
            Parameters.push_back (std::vector<Token>(InputTokens.Tokens.begin() + iPosition, InputTokens.Tokens.begin() + i)); // copy all tokens except the first one
            iPosition=i+1;
        }
    }
    if (iPosition<i) {
        Parameters.push_back(std::vector<Token>(InputTokens.Tokens.begin() + iPosition, InputTokens.Tokens.end())); // copy all tokens except the first one
    }
    return 0;
}

std::string Instruction::GetString() {
    std::string s="";
    if (ProgramLine>0) {
        s="   Line Number: " + std::to_string(ProgramLine) + "   -   \n\r";
    } else {
        s="   Direct  -    \n\r";
    }
    for (int i=0; i<Parameters.size(); i++){
        if (i>0) {
            s=s+ "   Extra Command :\n\r";
        }
        for (int j=0;j<Parameters[i].size(); j++) {
            s=s+ "       Type: " + std::to_string(Parameters[i][j].Type)+ " - ID: "+ std::to_string(Parameters[i][j].ID) + " - " + Parameters[i][j].Value + "\n\r";
        }
    }
    return s;
}