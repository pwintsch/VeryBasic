#include "processor.hpp"
#include "syntax.hpp"
#include "error.hpp"
#include "library.hpp"
#include "console.hpp"


Processor MyProcessor;


Processor::Processor() {
}

Processor::~Processor() {
}


int Processor::Addline(Instruction MyInstruction) {
    int i;
    if (Program.size()==0) {
        Program.push_back(MyInstruction);
        return NO_ERROR;
    } else {
        for (i=0; i<Program.size(); i++) {
            // if programline is equal replace instruction
            if (Program[i].ProgramLine == MyInstruction.ProgramLine) {
                return ERR_LINE_ALREADY_EXISTS;
            } else if (Program[i].ProgramLine > MyInstruction.ProgramLine) {
                Program.insert(Program.begin()+i, MyInstruction);
                return NO_ERROR;
            }
        }
    }
    Program.push_back(MyInstruction);
    return NO_ERROR;
}

int Processor::ChangeLine(Instruction MyInstruction) {
    int i;
    if (Program.size()==0) {
        Program.push_back(MyInstruction);
    } else {
        for (i=0; i<Program.size(); i++) {
            // if programline is equal replace instruction
            if (Program[i].ProgramLine == MyInstruction.ProgramLine) {
                Program[i]=MyInstruction;
                return NO_ERROR;
            }
        }
    }
    return ERR_NO_LINE_NUMBER;
}

std::string Processor::ListFull() {
    std::string s="";
    for (int i=0; i<Program.size(); i++) {
        s+=Program[i].GetString()+"\n\r";
    }
    return s;
}

std::string Processor::ListPartial(int StartLineNo, int EndLineNo) {
    std::string s="";
    for (int i=0; i<Program.size(); i++) {
        if (Program[i].ProgramLine>=StartLineNo && Program[i].ProgramLine<=EndLineNo) {
            s+=Program[i].GetString()+"\n\r";
        }
    }
    return s;
}

std::string Processor::Listline(int LineNo) {
    std::string s="";
    for (int i=0; i<Program.size(); i++) {
        if (Program[i].ProgramLine==LineNo) {
            s+=Program[i].GetString()+"\n\r";
            return s;
        }       
    }
    return s;
}


int Processor::ExecuteNextInstruction(){

    int r=NO_ERROR;
    bool NoBreakOrError=true;
    int i=0;
    Instruction MyInstruction=Program[CurrentLine];
    bool ConditionFailed=false;
    while (NoBreakOrError && i<MyInstruction.Commands.size() && !ConditionFailed) {                      
        if (MyInstruction.Commands[i].Type==tUserDefined) {
            r=LetCmd(MyInstruction.Commands[i]);
        } else { 
            r=CommandPtr[(MyInstruction.Commands[i].ID-CmdSep)](MyInstruction.Commands[i]);
        }
        if (r!=CMD_OK) {
            if (r==CMD_OK_Cond_Fail) {
                ConditionFailed=true;
            } else {
                return r;
            }
        }        
        i++;
    }
    return CMD_OK;
}


int Processor::Run() {
    CurrentLine=0;
    Variables.Clear();
    while (Active && CurrentLine<Program.size()) {
        int CommandResult=ExecuteNextInstruction();
        if (CommandResult==CMD_OK || CommandResult==CMD_OK_POINTER_CHANGE) {
            if (CommandResult==CMD_OK) {
                  CurrentLine++;
            }
        } else {
            return CommandResult;
        }
    }
    return CMD_OK;
}

int Processor::GotoLine(int LineNo) {
    for (int i=0; i<Program.size(); i++) {
        if (Program[i].ProgramLine>=LineNo) {
            CurrentLine=i;
            return NO_ERROR;
        }
    }
    return ERR_NO_LINE_NUMBER;
}

void Processor::Exit() {
    Active=false;

}


MyVariable::MyVariable () {
}
    
void MyVariable::Set(std::string pName, int pVarType, float pFValue, int pIValue, std::string pSValue){
        VariableType=pVarType;
        FltValue=pFValue;
        IntValue=pIValue;   
        StrValue=pSValue;   
        Name=pName;
 }

void VariableList::Clear() {
    VarList.clear();
}

VariableList::VariableList() {
}   

VariableList::~VariableList() {
    Clear();
}

int VariableList::Store (std::string Name, int VariableType, float FltValue, int IntValue, std::string StrValue) {
    if (VarList.find(Name)==VarList.end()) {
        MyVariable NewVariable;
        NewVariable.Set (Name, VariableType, FltValue, IntValue, StrValue);
        VarList[Name]=NewVariable;
        return NO_ERROR;
    } else {
        MyVariable NewVariable;
        NewVariable.Set (Name, VariableType, FltValue, IntValue, StrValue);
        VarList[Name]=NewVariable;
        return NO_ERROR;
    }
}

int VariableList::Get (std::string Name, int &VariableType, float &FltValue, int &IntValue, std::string &StrValue) {
    if (VarList.find(Name)==VarList.end()) {
        return ERR_VARIABLE_NOT_FOUND;
    } else {
        VariableType=VarList[Name].VariableType;
        FltValue=VarList[Name].FltValue;
        StrValue=VarList[Name].StrValue;
        IntValue=VarList[Name].IntValue;
        return NO_ERROR;
    }
}

std::string VariableList::ListVariables() {
    std::string s="";
    if (VarList.empty()) {
        s= "No variables declared yet \n\r";
        return s;
    }
    std::map<std::string, MyVariable>::iterator it;
    for (it=VarList.begin(); it!=VarList.end(); ++it) {
        s+=it->second.Name + " ";
        if (it->second.VariableType==tValue) {
            s+=std::to_string(it->second.FltValue);
        } else {
            s+=it->second.StrValue;
        }
        s+="\n\r";
    }
    return s;
}