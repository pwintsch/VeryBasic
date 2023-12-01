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
    int iCommandType=InputTokens.Tokens[iPosition].Type;   
    for (i=iPosition; i<InputTokens.Tokens.size(); i++){
        if (InputTokens.Tokens[i].ID == coColon && i>iPosition) {   
            int r=AddCommand (std::vector<Token>(InputTokens.Tokens.begin() + iPosition, InputTokens.Tokens.begin() + i)); // copy all tokens except the first one
            if (r!=NO_ERROR)  {
                return r;
            } else if  (ProgramLine!=0 && iCommandType==tDirectCommand) {
                return ERR_DIRECTCOMMAND_IN_PROGRAM;
            }
            iPosition=i+1;
        } else if (InputTokens.Tokens[i].ID == coTHEN && i>iPosition) {
            int r=AddCommand (std::vector<Token>(InputTokens.Tokens.begin() + iPosition, InputTokens.Tokens.begin() + i +1)); // copy all tokens except the first one
            if (r!=NO_ERROR)  {
                return r;
            }
            iPosition=i+1;
        }
    }
    if (iPosition<i) {
        int r=AddCommand (std::vector<Token>(InputTokens.Tokens.begin() + iPosition, InputTokens.Tokens.end())); // copy all tokens except the first one
        if (r!=NO_ERROR) {
            return r;
        } else if  (ProgramLine!=0 && iCommandType==tDirectCommand) {
            return ERR_DIRECTCOMMAND_IN_PROGRAM;
        }
    }
    return NO_ERROR;
}


int Instruction::AddCommand(std::vector<Token> pCommand) {
    // add a command to the instruction
    // return command type
    int r;
    
    if (pCommand[0].Type == tCommand || pCommand[0].Type == tDirectCommand || pCommand[0].Type == tVariable) {
        Command cmd;
        r=cmd.Initialise(pCommand);
        if (r!=NO_ERROR) {
            return r;
        }
        Commands.push_back(cmd);
    } else {
        return ERR_BAD_COMMAND;
    }
    
    return r;
}


std::string Instruction::GetString() {
    std::string s="";
    if (ProgramLine>0) {
        s="   Line Number: " + std::to_string(ProgramLine) + "   -   \n\r";
    } else {
        s="   Direct  -    \n\r";
    }
    for (int i=0; i<Commands.size(); i++){
        if (i>0) {
            s=s+ "   Extra Command :\n\r";
        }
        s=s+Commands[i].GetString();
    }
    return s;
}


Instruction::~Instruction() {
    // destructor
    Commands.clear();
}