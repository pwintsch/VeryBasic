#include "processor.hpp"
#include "syntax.hpp"
#include "error.hpp"
#include "library.hpp"
#include "console.hpp"


CallStack::CallStack() {
   Stack.clear();
}

CallStack::~CallStack() {
   Stack.clear();
}

int CallStack::Push(int LineNo, int CommandNo) {
    StackItem NewItem;
    NewItem.LineNo=LineNo;
    NewItem.CommandNo=CommandNo;
    Stack.push_back(NewItem);
    return NO_ERROR;
}


int CallStack::Pop(int &LineNo, int &CommandNo) {
    if (Stack.size()==0) {
        return ERR_CALLSTACK_EMPTY;
    } else {
        LineNo=Stack[Stack.size()-1].LineNo;
        CommandNo=Stack[Stack.size()-1].CommandNo;
        Stack.pop_back();
        return NO_ERROR;
    }
}


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
        LastLine=MyInstruction.ProgramLine;
        CurrentCommand=i;                   
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

int Processor::ResumeInstruction() {
    int r=NO_ERROR;
    bool NoBreakOrError=true;
    int i=CurrentCommand;
    Instruction MyInstruction=Program[CurrentLine];
    bool ConditionFailed=false;
    while (NoBreakOrError && i<MyInstruction.Commands.size() && !ConditionFailed) {   
        LastLine=MyInstruction.ProgramLine;
        CurrentCommand=i;                   
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


int Processor::Stop() {
    ProgramRunning=false;
    return CMD_OK;
}


void Processor::Clear() {
    Program.clear();
    Variables.Clear();
    ReturnStack.Stack.clear();
    CurrentLine=0;
    CurrentCommand=0;
    ResumeInstructionFlag=false;
    ProgramRunning=false;
}

int Processor::Run() {
    CurrentLine=0;
    Variables.Clear();
    ProgramRunning=true;
    ResumeInstructionFlag=false;
    int CommandResult=0;
    while (Active && ProgramRunning && CurrentLine<Program.size()) {
        if (ResumeInstructionFlag) {
            CommandResult=ResumeInstruction();
            ResumeInstructionFlag=false;
        } else {
            CommandResult=ExecuteNextInstruction();
        }
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
            CurrentCommand=0;
            return NO_ERROR;
        }
    }
    return ERR_NO_LINE_NUMBER;
}

void Processor::Exit() {
    Active=false;

}


MyVariable::MyVariable () {
    VariablePtr=NULL;
}
    
void MyVariable::Set(std::string pName, int pVarType, float pFValue, int pIValue, std::string pSValue){
        VariableType=pVarType;  
        Name=pName;
        switch (VariableType) {
            case cvDouble:
            case cvSingle:
                VariablePtr=new float(pFValue);
                break;
            case cvInteger:
                VariablePtr=new int(pIValue);
                break;
            case cvString:
                VariablePtr=new std::string(pSValue); 
                break;
            default:
                break;
        }
 }


void MyVariable::Update(std::string pName, int pVarType, float pFValue, int pIValue, std::string pSValue){
        VariableType=pVarType;   
        Name=pName;
        switch (VariableType) {
            case cvDouble:
            case cvSingle:
                *(float*)VariablePtr=pFValue;
                break;
            case cvInteger:
                *(int*)VariablePtr=pIValue;
                break;
            case cvString:
                *(std::string*)VariablePtr=pSValue; 
                break;
            default:
                break;
        }
}

void MyVariable::Get(std::string pName, int &pVarType, float &pFValue, int &pIValue, std::string &pSValue){
    pVarType=VariableType; 
    switch (VariableType) {
        case cvDouble:
        case cvSingle:
            pFValue=*(float*)VariablePtr;
            break;
        case cvInteger:
            pIValue=*(int*)VariablePtr;
            break;
        case cvString:
            pSValue=*(std::string*)VariablePtr; 
            break;
        default:
            break;
    }
}


MyVariable::~MyVariable() {

    if (VariablePtr!=NULL) {    
        switch (VariableType) {
            case cvDouble:
            case cvSingle:
                delete (float *)VariablePtr;
                break;
            case cvInteger:
                delete (int *)VariablePtr;
                break;
            case cvString:
                delete (std::string *)VariablePtr;
                break;
            default:
                break;
        }
    }
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
    auto item=VarList.find(Name);
    if (item==VarList.end()) {
        VarList[Name]=MyVariable();
        VarList[Name].Set (Name, VariableType, FltValue, IntValue, StrValue);
        return NO_ERROR;
    } else {
        item->second.Update(Name, VariableType, FltValue, IntValue, StrValue);    
        return NO_ERROR;
    }
}

int VariableList::Get (std::string Name, int &VariableType, float &FltValue, int &IntValue, std::string &StrValue) {
    auto item=VarList.find(Name);
    if (item==VarList.end()) {
        return ERR_VARIABLE_NOT_FOUND;
    } else {
        item->second.Get(Name, VariableType, FltValue, IntValue, StrValue);
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