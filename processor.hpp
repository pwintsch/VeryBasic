#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <vector>
#include <string>
#include <map>
#include "instruction.hpp"


class MyVariable {
public:
    std::string Name;
    // Variable type must be one of cvSingle, cvInteger, cvString, cvDouble
    int VariableType;
    void *VariablePtr;
    MyVariable ();
    ~MyVariable();

    void Set(std::string pName, int pVarType, float pFValue, int pIValue, std::string pSValue);
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
    bool DimensionsMatch(std::vector<int> &DimensionsToTest);
    ~MyArray();
//    void Set(std::string pName, int pVarType, float pFValue, int pIValue, std::string pSValue);
};



class VariableList {
public:
    std::map<std::string, MyVariable> VarList;
    VariableList();
    ~VariableList();
    int Store (std::string Name, int VariableType, float FltValue, int IntValue, std::string StrValue);
    int Get (std::string Name, int &VariableType, float &FltValue, int &IntValue, std::string &StrValue);
    bool Exists(std::string Name);
    void Clear();
    std::string ListVariables();
};


class ArrayList {
public:
    std::map<std::string, MyArray> ArrList;
    ArrayList();
    ~ArrayList();
    int Create(std::string Name, int VariableType, std::vector<int> &Dimensions);
    int Store (std::string Name, std::vector<int> &Dimensions, float FltValue, int IntValue, std::string StrValue);
    int Get (std::string Name, std::vector<int> &Dimensions, float &FltValue, int &IntValue, std::string &StrValue);
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
    std::vector<StackItem> Stack;
    CallStack();
    ~CallStack();
    int Push(int LineNo, int CommandNo);
    int Pop(int &LineNo, int &CommandNo);
};


class Processor {
private:
    std::vector<Instruction> Program;
    bool ProgramRunning;
public:
    int LastLine;
    int CurrentLine;
    int CurrentCommand;
    bool ResumeInstructionFlag;
    Processor();
    ~Processor();

    VariableList Variables;
    ArrayList Arrays;
    CallStack ReturnStack;
    bool Active=true;
    int Addline(Instruction MyInstruction);
    int ChangeLine(Instruction MyInstruction);
    int RemoveLine(int i);
    std::string ListFull();
    std::string ListPartial(int StartLineNo, int EndLineNo);
    std::string Listline(int LineNo);
    int GotoLine(int LineNo);
    int Stop();
    void Exit();
    void Clear();
    int Run();
    int SetVariable(CommandNode &Node, float FltValue, std::string StrValue);
    int GetVariable(CommandNode &Node, float &FltValue, int &IntValue, std::string &StrValue);
    int ExecuteNextInstruction();
    int ResumeInstruction();
};

extern Processor MyProcessor;



#endif // PROCESSOR_HPP

