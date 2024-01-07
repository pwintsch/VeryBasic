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

ForStack::ForStack() {
   Stack.clear();
}


ForStack::~ForStack() {
   Stack.clear();
}

int ForStack::Push(int LineNo, int CommandNo, std::string VariableName, float StartValue, float EndValue, float StepValue) {
    ForLoop NewItem;
    NewItem.LineNo=LineNo;
    NewItem.CommandNo=CommandNo;
    NewItem.VariableName=VariableName;
    NewItem.StartValue=StartValue;
    NewItem.EndValue=EndValue;
    NewItem.StepValue=StepValue;
    NewItem.CurrentValue=StartValue;
    Stack.push_back(NewItem);
    return NO_ERROR;
}


int ForStack::NextStep (std::string VariableName, int &LineNo, int &CommandNo) {
    if (Stack.size()==0) {
        return ERR_FORSTACK_EMPTY;
    } else {
        if (Stack[Stack.size()-1].VariableName==VariableName) {
            Stack[Stack.size()-1].CurrentValue+=Stack[Stack.size()-1].StepValue;
            if (Stack[Stack.size()-1].CurrentValue<=Stack[Stack.size()-1].EndValue) {
                LineNo=Stack[Stack.size()-1].LineNo;
                CommandNo=Stack[Stack.size()-1].CommandNo;
                return NO_ERROR;
            } else {
                Stack.pop_back();
                return NO_ERROR;
            }
        } else {
            return ERR_FORSTACK_MISMATCH;
        }
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
    Arrays.Clear();
    ReturnStack.Stack.clear();
    CurrentLine=0;
    CurrentCommand=0;
    ResumeInstructionFlag=false;
    ProgramRunning=false;
}

int Processor::Run() {
    CurrentLine=0;
    Variables.Clear();
    Arrays.Clear();
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
    return NO_ERROR;
}


int Processor::SetVariable(CommandNode &Node, float FltValue, std::string StrValue) {
    int r=0;
    int IntValue=int(FltValue);
    // Check to see if variable is variable or array item (subarguments.size()>0)
    if (Node.SubArguments.size()==0) {
        switch (Node.ID) {
            case cvInteger:
                MyProcessor.Variables.Store(Node.Value, cvInteger, 0,IntValue,  "");
                return NO_ERROR;
                break;
            case cvSingle:
            case cvDouble:
                MyProcessor.Variables.Store(Node.Value, Node.ID, FltValue, 0, "");
                return NO_ERROR;
                break;
            case cvString:
                MyProcessor.Variables.Store(Node.Value, cvString, 0, 0, StrValue);
                return NO_ERROR;
                break;
            default:
                return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
        }
    } else {
        // check to see if array exists
        if (MyProcessor.Arrays.Exists(Node.Value)) {
            std::vector<int> Dimensions;
            for (int i=0; i<Node.SubArguments.size(); i++) {
                int ResultType;
                float FltResult;
                std::string StrResult;
                r = Node.SubArguments[i].Evaluate(ResultType, FltResult,StrResult);
                if (r!=NO_ERROR) {
                    return r;
                }
                int DimensionPosition=int(FltResult);
                Dimensions.push_back(DimensionPosition);
            }
            int r=MyProcessor.Arrays.Store(Node.Value, Dimensions, FltValue, IntValue, StrValue);
            return r;
        } else {
            return ERR_ARRAY_ITEM_NOT_FOUND;
        }
    }
    return ERR_LAZY_PROGRAMMER;
}


int Processor::GetVariable(CommandNode &Node, float &FltValue, int &IntValue, std::string &StrValue) {
    int r=0;
    int VarType=0;
    FltValue=0;
    IntValue=0;
    StrValue="";
    // Check to see if variable is variable or array item (subarguments.size()>0)
    if (Node.SubArguments.size()==0) {
        r=MyProcessor.Variables.Get(Node.Value, VarType, FltValue, IntValue, StrValue);
        return r;
    } else {
        // check to see if array exists
        if (MyProcessor.Arrays.Exists(Node.Value)) {
            std::vector<int> Dimensions;
            for (int i=0; i<Node.SubArguments.size(); i++) {
                int ResultType;
                float FltResult;
                std::string StrResult;
                r = Node.SubArguments[i].Evaluate(ResultType, FltResult,StrResult);
                if (r!=NO_ERROR) {
                    return r;
                }
                int DimensionPosition=int(FltResult);
                Dimensions.push_back(DimensionPosition);
            }
            int r=MyProcessor.Arrays.Get(Node.Value, Dimensions, FltValue, IntValue, StrValue);
            return r;
        } else {
            return ERR_ARRAY_ITEM_NOT_FOUND;
        }
    }
    return ERR_LAZY_PROGRAMMER;
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


void MyVariable::Update(float pFValue, int pIValue, std::string pSValue){
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

void MyVariable::Get(float &pFValue, int &pIValue, std::string &pSValue){
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

std::string MyVariable::StringDesc() {
    std::string s="";
    s+=Name + " - ";
    switch (VariableType) {
        case cvDouble:
        case cvSingle:
            s+="Float : ";
            s+=std::to_string(*(float*)VariablePtr);
            break;
        case cvInteger:
            s+="Integer : ";
            s+=std::to_string(*(int*)VariablePtr);
            break;
        case cvString:
            s+="String : ";
            s+=*(std::string*)VariablePtr; 
            break;
        default:
            break;
    }
    return s;
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

bool MyArray::DimensionsMatch(std::vector<int> &DimensionsToTest, int Base) {
    if (DimensionsToTest.size()!=Dimensions.size()) {
        return false;
    }
    for (int i=0; i<Dimensions.size(); i++) {
        if ((DimensionsToTest[i]-Base)>=0 && (DimensionsToTest[i]-Base)<Dimensions[i]) {
            return true;
        }
    }
    return false;
}


MyArray::MyArray() {
    VariablePtr=NULL;
}



void MyArray::Define (std::string pName, int pVarType, std::vector<int> &Dimensions) {
    VariableType=pVarType;   
    Name=pName;
    VariablePtr=NULL;
    this->Dimensions=Dimensions;
    TotalSize=1;
    for (int i=0; i<Dimensions.size(); i++) {
        TotalSize*=Dimensions[i];
    }
    switch (VariableType) {
        case cvDouble:
        case cvSingle:
            VariablePtr=new float[TotalSize];
            break;
        case cvInteger:
            VariablePtr=new int[TotalSize];
            break;
        case cvString:
            VariablePtr=new std::string[TotalSize]; 
            break;
        default:
            break;
    }
}


MyArray::~MyArray() {
    if (VariablePtr!=NULL) {    
        switch (VariableType) {
            case cvDouble:
            case cvSingle:
                delete[] (float *)VariablePtr;
                break;
            case cvInteger:
                delete[] (int *)VariablePtr;
                break;
            case cvString:
                delete[] (std::string *)VariablePtr;
                break;
            default:
                break;
        }
    }
}



VariableList::VariableList() {
}   


void VariableList::Clear() {
    VarList.clear();
}

bool VariableList::Exists(std::string Name) {
    auto item=VarList.find(Name);
    if (item==VarList.end()) {
        return false;
    } else {
        return true;
    }
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
        item->second.Update(FltValue, IntValue, StrValue);    
        return NO_ERROR;
    }
}

int VariableList::Get (std::string Name, int &VariableType, float &FltValue, int &IntValue, std::string &StrValue) {
    auto item=VarList.find(Name);
    if (item==VarList.end()) {
        return ERR_VARIABLE_NOT_FOUND;
    } else {
        item->second.Get(FltValue, IntValue, StrValue);
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
        s+= it->second.StringDesc() + "\n\r";
    }
    return s;
}


ArrayList::ArrayList() {
}

void ArrayList::Clear() {
    ArrList.clear();
}

bool ArrayList::Exists(std::string Name) {
    auto item=ArrList.find(Name);
    if (item==ArrList.end()) {
        return false;
    } else {
        return true;
    }
}

int ArrayList::Create(std::string Name, int VariableType, std::vector<int> &Dimensions) {
    auto item=ArrList.find(Name);
    if (item==ArrList.end()) {

        ArrList[Name]=MyArray();
        ArrList[Name].Define(Name, VariableType, Dimensions);
        return NO_ERROR;
    } else {
        return ERR_DIM_ARRAY_ALREADY_EXISTS;
    }
}

int ArrayList::Store (std::string Name, std::vector<int> &Dimensions, float FltValue, int IntValue, std::string StrValue) {
    auto item=ArrList.find(Name);
    if (item==ArrList.end()) {
        return ERR_VARIABLE_NOT_FOUND;
    } else {
        int Index=0;
        int Multiplier=1;
        for (int i=Dimensions.size()-1; i>=0; i--) {
            Index+=(Dimensions[i]-DimensionBase)*Multiplier;
            Multiplier*=item->second.Dimensions[i];
        }
        if (item->second.DimensionsMatch(Dimensions, DimensionBase)) {
            switch (item->second.VariableType) {
                case cvDouble:
                case cvSingle:
                    ((float*)item->second.VariablePtr)[Index]=FltValue;
                    break;
                case cvInteger:
                    ((int*)item->second.VariablePtr)[Index]=IntValue;
                    break;
                case cvString:
                    ((std::string*)item->second.VariablePtr)[Index]=StrValue; 
                    break;
                default:
                    break;
            }
            return NO_ERROR;
        } else {
            return ERR_DIM_ARRAY_DONT_MATCH;
        }
    }
}


int ArrayList::Get (std::string Name, std::vector<int> &Dimensions, float &FltValue, int &IntValue, std::string &StrValue) {
    auto item=ArrList.find(Name);
    if (item==ArrList.end()) {
        return ERR_VARIABLE_NOT_FOUND;
    } else {
        int Index=0;
        int Multiplier=1;
        for (int i=Dimensions.size()-1; i>=0; i--) {
            Index+=(Dimensions[i]-DimensionBase)*Multiplier;
            Multiplier*=item->second.Dimensions[i];
        }
        if (item->second.DimensionsMatch(Dimensions, DimensionBase)) {
            switch (item->second.VariableType) {
                case cvDouble:
                case cvSingle:
                    FltValue=((float*)item->second.VariablePtr)[Index];
                    break;
                case cvInteger:
                    IntValue=((int*)item->second.VariablePtr)[Index];
                    break;
                case cvString:
                    StrValue=((std::string*)item->second.VariablePtr)[Index]; 
                    break;
                default:
                    break;
            }
            return NO_ERROR;
        } else {
            return ERR_DIM_ARRAY_DONT_MATCH;
        }
    }
}


std::string ArrayList::ListArrays() {
    std::string s="";
    if (ArrList.empty()) {
        s= "No arrays declared yet \n\r";
        return s;
    }
    std::map<std::string, MyArray>::iterator it;
    for (it=ArrList.begin(); it!=ArrList.end(); ++it) {
        s+= it->second.Name + " - ";
        switch (it->second.VariableType) {
            case cvDouble:
            case cvSingle:
                s+="Float : ";
                break;
            case cvInteger:
                s+="Integer : ";
                break;
            case cvString:
                s+="String : ";
                break;
            default:
                break;
        }
        s+=std::to_string(it->second.Dimensions.size()) + " dimensions (";
        for (int i=0; i<it->second.Dimensions.size(); i++) {
            s+=std::to_string(it->second.Dimensions[i]);
            if (i<it->second.Dimensions.size()-1) {
                s+=", ";
            }
        }
        s+=")\n\r";
    }
    return s;
}


void ArrayList::SetBase(int Size) {
    DimensionBase=Size;
}


ArrayList::~ArrayList() {
    Clear();
}
