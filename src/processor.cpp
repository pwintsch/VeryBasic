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
    Stack.push_back(NewItem);
    return NO_ERROR;
}


RepeatStack::RepeatStack() {
   Stack.clear();
}

RepeatStack::~RepeatStack() {
   Stack.clear();
}

int RepeatStack::Push(int LineNo, int CommandNo) {
    RepeatLoop NewItem;
    NewItem.LineNo=LineNo;
    NewItem.CommandNo=CommandNo;
    Stack.push_back(NewItem);
    return NO_ERROR;
}

int RepeatStack::CurrentRepeatStart(int &LineNo, int &CommandNo) {
    if (Stack.size()==0) {
        return ERR_REPEATSTACK_EMPTY;
    } else {
        LineNo=Stack[Stack.size()-1].LineNo;
        CommandNo=Stack[Stack.size()-1].CommandNo;
        return NO_ERROR;
    }
}

int RepeatStack::Pop() {
    if (Stack.size()==0) {
        return ERR_REPEATSTACK_EMPTY;
    } else {
        Stack.pop_back();
        return NO_ERROR;
    }
}

WhileStack::WhileStack() {
   Stack.clear();
}

int WhileStack::Push(int LineNo, int CommandNo, CommandNode &WhileExpression) {
    WhileLoop NewItem;
    NewItem.LineNo=LineNo;
    NewItem.CommandNo=CommandNo;
    NewItem.Expression=WhileExpression;
    Stack.push_back(NewItem);
    return NO_ERROR;
}

int WhileStack::CurrentWhileStart(int &LineNo, int &CommandNo) {
    if (Stack.size()==0) {
        return ERR_WHILESTACK_EMPTY;
    } else {

            LineNo=Stack[Stack.size()-1].LineNo;
            CommandNo=Stack[Stack.size()-1].CommandNo;
        return NO_ERROR;
    }
}

int WhileStack::CurrentWhileExpression( float &FltValue) {
    if (Stack.size()==0) {
        return ERR_WHILESTACK_EMPTY;
    } else {
        CommandNode WhileExpression=Stack[Stack.size()-1].Expression;
        int ResultType;
        float FltResult;
        std::string StrResult;
        int r=WhileExpression.Evaluate(ResultType, FltResult, StrResult);
        if (r!=NO_ERROR) {
            return r;
        }
        FltValue=FltResult;
        return NO_ERROR;
    }
}

int WhileStack::Pop() {
    if (Stack.size()==0) {
        return ERR_REPEATSTACK_EMPTY;
    } else {
        Stack.pop_back();
        return NO_ERROR;
    }
}


WhileStack::~WhileStack() {
   Stack.clear();
}


int ForStack::NextStep (std::string VariableName, float &CurrentValue, bool &Loop, int &LineNo, int &CommandNo) {
    if (Stack.size()==0) {
        return ERR_FORSTACK_EMPTY;
    } else {
        if (Stack[Stack.size()-1].VariableName==VariableName) {
            CurrentValue+=Stack[Stack.size()-1].StepValue;
            if (Stack[Stack.size()-1].StepValue>0 &&  CurrentValue<=Stack[Stack.size()-1].EndValue) {
                LineNo=Stack[Stack.size()-1].LineNo;
                CommandNo=Stack[Stack.size()-1].CommandNo;
                Loop=true;
                return NO_ERROR;
            } else if (Stack[Stack.size()-1].StepValue<0 &&  CurrentValue>=Stack[Stack.size()-1].EndValue) {
                LineNo=Stack[Stack.size()-1].LineNo;
                CommandNo=Stack[Stack.size()-1].CommandNo;
                Loop=true;
                return NO_ERROR;
            } else {
                Stack.pop_back();
                Loop=false;
                return NO_ERROR;
            }
        } else {
            return ERR_FORSTACK_MISMATCH;
        }
    }
}


Processor MyProcessor;


Processor::Processor() {
    Active=true;
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
            s+=Program[i].GetString();
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
    while (NoBreakOrError && i<MyInstruction.Commands.size()) {   
        LastLine=MyInstruction.ProgramLine;
        CurrentCommand=i;                   
        if (MyInstruction.Commands[i].Type==tUserDefined) {
            r=LetCmd(MyInstruction.Commands[i]);
        } else { 
            r=CommandPtr[(MyInstruction.Commands[i].ID-CmdSep)](MyInstruction.Commands[i]);
        }
        NoOfInstructionsExecuted++;
        if (BreakEnabled) {
            if (NoOfInstructionsExecuted%1000 == 0) {
                if (Terminal.CheckKeyPress()==CTRL_KEY('z')) return CMD_BREAK;
            }
        }
        if (r!=CMD_OK) {
            if (r==CMD_OK_Cond_Fail) {
                r=CMD_OK;
                // check to find an ELSE command in instruction
                while (i<MyInstruction.Commands.size() && MyInstruction.Commands[i].ID!=coELSE) {
                    i++;
                }
            } else {
                return r;
            }
        }  else {
            if (i+1<MyInstruction.Commands.size() && MyInstruction.Commands[i+1].ID==coELSE) {
                i=MyInstruction.Commands.size();
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
    Variables.Clear();
    Arrays.Clear();
    ReturnStack.Stack.clear();
    CurrentLine=0;
    CurrentCommand=0;
    ResumeInstructionFlag=false;
    ProgramRunning=false;
    ReadLastInstruction=0;
    ReadLastCommand=0;
    ReadLastArgument=0;
    SearchNextDataStatement=true;
}

void Processor::Reset() {
    Clear();
    Program.clear();
}


int Processor::Run(bool Reset) {
    if (Reset) {
        CurrentLine=0;
        Variables.Clear();
        Arrays.Clear();
    }
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


int Processor::DetailedLine(int LineNo, std::string &LineText) {
    LineText="";
    for (int i=0; i<Program.size(); i++) {
        if (Program[i].ProgramLine==LineNo) {
            LineText+=Program[i].GetDetailedString();
            return NO_ERROR;
        }
    }
    return ERR_NO_LINE_NUMBER;
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


int Processor::NewForLoop(CommandNode &Variable, float StartValue, float EndValue, float StepValue) {
    int r=ForLoopStack.Push(CurrentLine, CurrentCommand, Variable.Value, StartValue, EndValue, StepValue);
    if (r!=NO_ERROR) {
        return r;
    }
    r=SetVariable(Variable, StartValue, "");
    return r;
}


int Processor::NextForLoop(CommandNode &Variable, bool &Loop) {

    float CurrentValue=0;
    int IntValue=0;
    std::string s="";
    int r=GetVariable(Variable,CurrentValue, IntValue, s);
    if (r!=NO_ERROR) {
        return r;
    }
    int TmpLineIndex=0;
    int TmpCommandIndex=0;
    r=ForLoopStack.NextStep(Variable.Value, CurrentValue, Loop, TmpLineIndex, TmpCommandIndex);
    if (r!=NO_ERROR) {
        return r;
    }
    if (Loop) {
        CurrentLine=TmpLineIndex;
        CurrentCommand=TmpCommandIndex+1;
    } else {
        return NO_ERROR;
    }
    r=SetVariable(Variable, CurrentValue, "");
    return NO_ERROR;
}


void Processor::ResetReadCmdData() {
    ReadLastInstruction=0;
    ReadLastCommand=0;
    ReadLastArgument=0;
    SearchNextDataStatement=true;
}


int Processor::SetNextReadCmdData(int DestinationLineNo) {
    ReadLastInstruction=0;
    ReadLastCommand=0;
    ReadLastArgument=0;
    SearchNextDataStatement=true;
    for (int i=0; i<Program.size(); i++) {
        if (Program[i].ProgramLine==DestinationLineNo) {
            ReadLastInstruction=i;
            return NO_ERROR;
        }
    }
    return ERR_BAD_RESTORE_LINE;
}


int Processor::GetNextReadCmdData(int &ReturnType, float &FLtValue, std::string &StrValue) {
bool ItemRead=false;
    if (Program.size()>0) {
        while (!ItemRead && ReadLastInstruction<Program.size()) {
            if (SearchNextDataStatement) {
                if (ReadLastCommand<Program[ReadLastInstruction].Commands.size()) {
                    if (Program[ReadLastInstruction].Commands[ReadLastCommand].ID==coDATA) {
                        SearchNextDataStatement=false;
                    } else {
                        ReadLastCommand++;
                    }
                } else {
                    ReadLastCommand=0;
                    ReadLastInstruction++;      
                }
            } else if (ReadLastInstruction<Program.size()) {
                if (ReadLastCommand<Program[ReadLastInstruction].Commands.size()) {
                    if (ReadLastArgument<Program[ReadLastInstruction].Commands[ReadLastCommand].Arguments.size()) {
                        ReturnType=Program[ReadLastInstruction].Commands[ReadLastCommand].Arguments[ReadLastArgument].Type;
                        if (Program[ReadLastInstruction].Commands[ReadLastCommand].Arguments[ReadLastArgument].Type==tString) {
                            StrValue=Program[ReadLastInstruction].Commands[ReadLastCommand].Arguments[ReadLastArgument].Value;
                        } else {
                            FLtValue=std::stof(Program[ReadLastInstruction].Commands[ReadLastCommand].Arguments[ReadLastArgument].Value);
                        }
                        ItemRead=true;
                        ReadLastArgument=ReadLastArgument+2;
                        return NO_ERROR;
                    } else {
                        ReadLastArgument=0;
                        ReadLastCommand++;
                        SearchNextDataStatement=true;
                    }
                } else {
                    ReadLastArgument=0;
                    ReadLastCommand=0;
                    ReadLastInstruction++;
                    SearchNextDataStatement=true;         
                }

            } else {
                return ERR_NO_READ_DATA;
            }
        }

    } else {
        return ERR_NO_READ_DATA;
    }
    return ERR_NO_READ_DATA;
}


int Processor::ExecuteFunction(CommandNode &Node,int &ReturnType, float &FltValue, std::string &StrValue) {

    int r=FunctionPtr [Node.ID-FuncSep](Node, ReturnType, FltValue, StrValue);
    return r;
}


int Processor::AddDefFn(std::string Name, CommandNode &Header, CommandNode &Body) {
    int r=Functions.AddFunction(Name, Header, Body);
    return r;
}


int Processor::EvaluateDEFFN(CommandNode &Node, int &ReturnType, float &FltValue, std::string &StrValue) {
    int r=Functions.CalcFunction(Node, ReturnType, FltValue, StrValue);
    return r;
}


int Processor::LastProgramLine() {
    if (Program.size()>CurrentLine) {
        return Program[CurrentLine].ProgramLine;
    } else {
        return 0;
    }
}


int Processor::SetProgramPointersToNextWEND() {
    int i=CurrentLine;
    int j=CurrentCommand;
    bool Found=false;
    while (i<Program.size() && !Found) {
        while (j<Program[i].Commands.size() && !Found) {
            if (Program[i].Commands[j].ID==coWEND) {
                Found=true;
                CurrentLine=i;
                CurrentCommand=j;
                ResumeInstructionFlag=true;
                return NO_ERROR;
            }
            j++;
        }
        j=0;
        i++;
    }
    return ERR_WEND_NOT_FOUND;
}

void Processor::Exit() {
    Active=false;

}


MyVariable::MyVariable () {
//    VariablePtr=NULL;
    LocalContext=0;
}
    
void MyVariable::Set(std::string pName, int pVarType, float pFValue, int pIValue, std::string pSValue, int pContext){
        VariableType=pVarType;  
        Name=pName;
        LocalContext=pContext;
        switch (VariableType) {
            case cvDouble:
            case cvSingle:
//                VariablePtr=new float(pFValue);
                Value=pFValue;
                break;
            case cvInteger:
//                VariablePtr=new int(pIValue);
                Value=pIValue;
                break;
            case cvString:
//                VariablePtr=new std::string(pSValue); 
                Value=pSValue;
                break;
            default:
                break;
        }
 }


void MyVariable::Update(float pFValue, int pIValue, std::string pSValue){
        switch (VariableType) {
            case cvDouble:
            case cvSingle:
//                *(float*)VariablePtr=pFValue;
                Value=pFValue;
                break;
            case cvInteger:
//                *(int*)VariablePtr=pIValue;
                Value=pIValue;
                break;
            case cvString:
//                *(std::string*)VariablePtr=pSValue; 
                Value=pSValue;
                break;
            default:
                break;
        }
}

void MyVariable::Get(float &pFValue, int &pIValue, std::string &pSValue){
    switch (VariableType) {
        case cvDouble:
        case cvSingle:
//            pFValue=*(float*)VariablePtr;
            pFValue=std::any_cast<float>(Value);
            break;
        case cvInteger:
//            pIValue=*(int*)VariablePtr;
            pIValue=std::any_cast<int>(Value);
            break;
        case cvString:
//            pSValue=*(std::string*)VariablePtr; 
            pSValue=std::any_cast<std::string>(Value);
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
//            s+=std::to_string(*(float*)VariablePtr); 
            s+=std::to_string(std::any_cast<float>(Value));
            break;
        case cvInteger:
            s+="Integer : ";
//            s+=std::to_string(*(int*)VariablePtr);
            s+=std::to_string(std::any_cast<int>(Value));
            break;
        case cvString:
            s+="String : ";
//            s+=*(std::string*)VariablePtr;
            s+=std::any_cast<std::string>(Value); 
            break;
        default:
            break;
    }
    return s;
}


MyVariable::~MyVariable() {
/*
    if (VariablePtr!=NULL) {    
        switch (VariableType) {
            case cvDouble:
            case cvSingle:
//                delete (float *)VariablePtr;
                break;
            case cvInteger:
//                delete (int *)VariablePtr;
                break;
            case cvString:
//                delete (std::string *)VariablePtr;
                break;
            default:
                break;
        }
    }*/
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
    VariableContext=0;
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
        VarList[Name].Set (Name, VariableType, FltValue, IntValue, StrValue, VariableContext);
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

void VariableList::IncreaseContext() {
    VariableContext++;
}

void VariableList::RemoveContext() {

    auto item=VarList.begin();
    while (item!=VarList.end()) {
        if (item->second.LocalContext==VariableContext) {
            item=VarList.erase(item);
        } else {
            item++;
        }
    }
    VariableContext--;
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


int DefFnStack::AddFunction(std::string Name, CommandNode &Header, CommandNode &Body) {
    auto item=FunctionMap.find(Name);
    if (item==FunctionMap.end()) {
        DefFN Function;
        Function.Name=Name;
        Function.FunctionHeader.InitialiseFromCommandNode(Header);
        Function.FunctionBody.InitialiseFromCommandNode(Body);
        FunctionMap[Name]=Function;
        return NO_ERROR;
    } else {
        return ERR_FUNCTION_ALREADY_EXISTS;
    }
}

int DefFnStack::CalcFunction(CommandNode &Node, int &ReturnType, float &FltValue, std::string &StrValue) {
    auto item=FunctionMap.find(Node.Value);
    if (item==FunctionMap.end()) {
        return ERR_FUNCTION_NOT_FOUND;
    } else {
        // check to see if number of arguments match
        if (Node.SubArguments.size()!=item->second.FunctionHeader.SubArguments.size()) {
            return ERR_BAD_FUNCTION_PARAM_NO;
        }
        MyProcessor.Variables.IncreaseContext();
        for (int i=0; i<Node.SubArguments.size(); i++) {
            int LocalReturnType=0;
            float LocalFltValue=0;
            std::string LocalStrValue="";
            int r=Node.SubArguments[i].Evaluate(LocalReturnType, LocalFltValue, LocalStrValue);
            if (r!=NO_ERROR) {
                return r;
            }
            MyProcessor.SetVariable(item->second.FunctionHeader.SubArguments[i], LocalFltValue, LocalStrValue);
        }
        // Check to see if argument types match
      /*  for (int i=0; i<Node.SubArguments.size(); i++) {
            if (Node.SubArguments[i].ReturnType!=item->second.FunctionHeader.SubArguments[i].ReturnType) {
                return ERR_BAD_FUNCTION_PARAM_TYPE;
            }
        }   */
        int r=item->second.FunctionBody.Evaluate(ReturnType, FltValue, StrValue);
        MyProcessor.Variables.RemoveContext();
        return r;
    }
}