#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <vector>
#include <string>
#include <map>
#include <any>
#include "instruction.hpp"


class MyVariable {
public:
    std::string Name;
    // Variable type must be one of cvSingle, cvInteger, cvString, cvDouble
    int VariableType;
    int LocalContext;
//    void *VariablePtr;
    std::any Value;

    MyVariable ();
    ~MyVariable();

    void Set(std::string pName, int pVarType, float pFValue, int pIValue, std::string pSValue, int pContext);
    void Update(float pFValue, int pIValue, std::string pSValue);
    void Get( float &pFValue, int &pIValue, std::string &pSValue);
    std::string StringDesc();
};


class MyArray {
public:
    std::string Name;
    // Variable type must be one of cvSingle, cvInteger, cvString, cvDouble
    int VariableType;
    void *VariablePtr=NULL;
    std::vector<int> Dimensions;
    int TotalSize=0;
    MyArray();
    void Define (std::string pName, int pVarType, std::vector<int> &Dimensions);
    bool DimensionsMatch(std::vector<int> &DimensionsToTest, int Base);
    ~MyArray();
//    void Set(std::string pName, int pVarType, float pFValue, int pIValue, std::string pSValue);
};



class VariableList {
public:
    std::map<std::string, MyVariable> VarList;
    int VariableContext=0;
    VariableList();
    ~VariableList();
    int Store (std::string Name, int VariableType, float FltValue, int IntValue, std::string StrValue);
    int Get (std::string Name, int &VariableType, float &FltValue, int &IntValue, std::string &StrValue);
    bool Exists(std::string Name);
    void Clear();
    void IncreaseContext();
    void RemoveContext();
    std::string ListVariables();
};


class ArrayList {
private:
    int DimensionBase=0;
public:
    std::map<std::string, MyArray> ArrList;
    ArrayList();
    ~ArrayList();
    int Create(std::string Name, int VariableType, std::vector<int> &Dimensions);
    int Store (std::string Name, std::vector<int> &Dimensions, float FltValue, int IntValue, std::string StrValue);
    int Get (std::string Name, std::vector<int> &Dimensions, float &FltValue, int &IntValue, std::string &StrValue);
    void SetBase(int Size);
    bool Exists(std::string Name);
    void Clear();
    std::string ListArrays();
};


struct StackItem {
    int LineNo;
    int CommandNo;
};


class CallStack {
    public:
    std::vector<StackItem> CStack;
    CallStack();
    ~CallStack();
    int Push(int LineNo, int CommandNo);
    int Pop(int &LineNo, int &CommandNo);
};


struct ForLoop {
    int LineNo;
    int CommandNo;
    std::string VariableName;
    float StartValue;
    float EndValue;
    float StepValue;
};


class ForStack {
    public:
    std::vector<ForLoop> FStack;
    ForStack();
    ~ForStack();
    int Push(int LineNo, int CommandNo, std::string VariableName, float StartValue, float EndValue, float StepValue);
    int NextStep (std::string VariableName, float &CurrentValue, bool &Loop, int &LineNo, int &CommandNo);
};

struct RepeatLoop {
    int LineNo;
    int CommandNo;
};

class RepeatStack {
    public:
    std::vector<RepeatLoop> RStack;
    RepeatStack();
    ~RepeatStack();
    int Push(int LineNo, int CommandNo);
    int CurrentRepeatStart(int &LineNo, int &CommandNo);
    int Pop();
};


struct WhileLoop {
    int LineNo;
    int CommandNo;
    CommandNode Expression;
};

class WhileStack {
    public:
    std::vector<WhileLoop> WStack;
    WhileStack();
    ~WhileStack();
    int Push(int LineNo, int CommandNo, CommandNode &WhileExpression);
    int CurrentWhileStart(int &LineNo, int &CommandNo);
    int CurrentWhileExpression(float &FltValue);
    int Pop();
};



struct DefFN {
    std::string Name;
    CommandNode FunctionHeader;
    CommandNode FunctionBody;
};


class DefFnStack {
    public:
        std::map<std::string, DefFN> FunctionMap;
        int AddFunction(std::string Name, CommandNode &Header, CommandNode &Body);
        int CalcFunction(CommandNode &Node, int &ReturnType, float &FltValue, std::string &StrValue);
        // local variables need to be set before calling CalcFunction
};

class Processor {
private:
    std::vector<Instruction> Program;
    bool ProgramRunning;
    int ReadLastInstruction=0;
    int ReadLastCommand=0;
    int ReadLastArgument=0;
    bool SearchNextDataStatement=true;
    int NoOfInstructionsExecuted=0;
    bool BreakEnabled=true;
    bool PrintRPN=false;

public:
    bool TraceExecs=false;
    int LastLine;
    int CurrentLine;
    int CurrentCommand;
    bool ResumeInstructionFlag;
    Processor();
    ~Processor();

    VariableList Variables;
    ArrayList Arrays;
    DefFnStack Functions;
    CallStack ReturnStack;
    ForStack ForLoopStack;
    RepeatStack RepeatLoopStack;
    WhileStack WhileLoopStack;
    bool Active=true;
    void SetPrintRPN(bool Value);
    bool GetPrintRPN();
    int Addline(Instruction MyInstruction);
    int ChangeLine(Instruction MyInstruction);
    int RemoveLine(int LineNo);
    std::string ListFull();
    std::string ListPartial(int StartLineNo, int EndLineNo);
    std::string Listline(int LineNo);
    int GotoLine(int LineNo);
    int DetailedLine(int LineNo, std::string &LineText);
    int Stop();
    void Exit();
    void Clear();
    void Reset();
    int Run(bool Reset);
    int AddDefFn(std::string Name, CommandNode &Header, CommandNode &Body);
    int EvaluateDEFFN(CommandNode &Node, int &ReturnType, float &FltValue, std::string &StrValue);
    int SetVariable(CommandNode &Node, float FltValue, std::string StrValue);
    int CheckVariable(CommandNode &Node);
    int GetVariable(CommandNode &Node, float &FltValue, int &IntValue, std::string &StrValue);
    int NewForLoop(CommandNode &Variable, float StartValue, float EndValue, float StepValue);
    int NextForLoop(CommandNode &Variable, bool &Loop);
    int GetNextReadCmdData(int &ReturnType, float &FltValue, std::string &StrValue);
    int SetNextReadCmdData(int DestinationLineNo);
    int SetProgramPointersToNextWEND();
    void ResetReadCmdData();
    int ExecuteNextInstruction();
    int ResumeInstruction();
    int ExecuteFunction(CommandNode &Node,int &ReturnType, float &FltValue, std::string &StrValue);
    int LastProgramLine();
};

extern Processor MyProcessor;



#endif // PROCESSOR_HPP

