#include "library.hpp"
#include "console.hpp"
#include "processor.hpp"
#include "error.hpp"





int EmptyCmd(Command MyCommand)
{
    Terminal.WriteLn("EmptyCmd");
    return NO_ERROR;
}


int ListCmd(Command MyCommand)
{   
    if (MyCommand.RuleNo==0) {
        std::string s=MyProcessor.ListFull();
        Terminal.WriteLn(s.c_str());
        return NO_ERROR;
    } else if (MyCommand.RuleNo==1) {
        int LineNo= stoi(MyCommand.Arguments[0].Value);
        std::string s=MyProcessor.Listline(LineNo);
        Terminal.WriteLn(s.c_str());
        return NO_ERROR;
    } else if (MyCommand.RuleNo==2) {
        int StartLineNo= stoi(MyCommand.Arguments[0].Value);
        int EndLineNo= stoi(MyCommand.Arguments[2].Value);
        std::string s=MyProcessor.ListPartial(StartLineNo, EndLineNo);
        Terminal.WriteLn(s.c_str());
        return NO_ERROR;
    } else {
        return ERR_SYNTAX;
    }
}

int NewCmd(Command MyCommand)
{
    Terminal.WriteLn("NewCmd");
    return NO_ERROR;
}

int ExitCmd(Command MyCommand)
{
    MyProcessor.Exit();
    return NO_ERROR;
}


int DebugCmd(Command MyCommand)
{
    Terminal.WriteLn("DebugCmd");
    return NO_ERROR;
}


int RunCmd(Command MyCommand)
{
    Terminal.WriteLn("RunCmd");
    return NO_ERROR;
}


int EvalCmd(Command MyCommand)
{
    std::string s="Eval expression: " + MyCommand.Arguments[0].GetString();
    Terminal.WriteLn(s.c_str());
    s=MyCommand.GetDetailedString();
    Terminal.WriteLn(s.c_str());
    float NumResult;
    std::string StrResult;  
    int r=MyCommand.Arguments[0].Evaluate(NumResult, StrResult);
    if (r==NO_ERROR) {
        Terminal.WriteLn(StrResult.c_str());
        Terminal.WriteFStringLn("Result: %f", NumResult);
    }
    
    return r;
}


int LoadCmd(Command MyCommand)
{
    Terminal.WriteLn("LoadCmd");
    return NO_ERROR;
}


int SaveCmd(Command MyCommand)
{
    Terminal.WriteLn("SaveCmd");
    return NO_ERROR;
}

int NodelistCmd(Command MyCommand)
{
    Terminal.WriteLn("NodelistCmd");
    return NO_ERROR;
}

int EditCmd(Command MyCommand)
{
    Terminal.WriteLn("EditCmd");
    return NO_ERROR;
}

int EditorCmd(Command MyCommand)
{
    Terminal.WriteLn("EditorCmd");
    return NO_ERROR;
}


int (*DirectCommandPtr[])(Command MyCommand) = { EmptyCmd, ListCmd, NewCmd, ExitCmd, DebugCmd, RunCmd, EvalCmd, LoadCmd, SaveCmd, NodelistCmd, EditCmd, EditorCmd } ;


