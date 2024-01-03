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
    float FltValue;
    int IntValue;
    std::string StrValue;
    MyVariable ();
    void Set(std::string pName, int pVarType, float pFValue, int pIValue, std::string pSValue);
};


class VariableList {
public:
    std::map<std::string, MyVariable> VarList;
    VariableList();
    ~VariableList();
    int Store (std::string Name, int VariableType, float FltValue, int IntValue, std::string StrValue);
    int Get (std::string Name, int &VariableType, float &FltValue, int &IntValue, std::string &StrValue);
    void Clear();
    std::string ListVariables();
};


class Processor {
private:
    std::vector<Instruction> Program;
    int CurrentLine;
public:
    Processor();
    ~Processor();

    VariableList Variables;
    bool Active=true;
    int Addline(Instruction MyInstruction);
    int ChangeLine(Instruction MyInstruction);
    int RemoveLine(int i);
    std::string ListFull();
    std::string ListPartial(int StartLineNo, int EndLineNo);
    std::string Listline(int LineNo);
    int GotoLine(int LineNo);
    void Exit();
    int Run();
    int ExecuteNextInstruction();
};

extern Processor MyProcessor;



#endif // PROCESSOR_HPP

