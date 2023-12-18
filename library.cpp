#include "library.hpp"
#include "console.hpp"
#include "processor.hpp"
#include "error.hpp"

int test1()
{
   return 101;
}

int test2()
{
   return 202;
}

int test3()
{
   return 303;
}

int (*functptr[])() = { test1, test2, test3 } ;

int EmptyCmd(Command MyCommand)
{
    Terminal.WriteLn("EmptyCmd");
    return 0;
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
    return 0;
}

int ExitCmd(Command MyCommand)
{
    MyProcessor.Exit();
    return NO_ERROR;
}


int DebugCmd(Command MyCommand)
{
    Terminal.WriteLn("DebugCmd");
    return 0;
}


int RunCmd(Command MyCommand)
{
    Terminal.WriteLn("RunCmd");
    return 0;
}


int EvalCmd(Command MyCommand)
{
    std::string s="Eval expression: " + MyCommand.Arguments[0].GetString();
    Terminal.WriteLn(s.c_str());
    return NO_ERROR;
}


int LoadCmd(Command MyCommand)
{
    Terminal.WriteLn("LoadCmd");
    return 0;
}


int SaveCmd(Command MyCommand)
{
    Terminal.WriteLn("SaveCmd");
    return 0;
}

int NodelistCmd(Command MyCommand)
{
    Terminal.WriteLn("NodelistCmd");
    return 0;
}

int EditCmd(Command MyCommand)
{
    Terminal.WriteLn("EditCmd");
    return 0;
}

int EditorCmd(Command MyCommand)
{
    Terminal.WriteLn("EditorCmd");
    return 0;
}


int (*DirectCommandPtr[])(Command MyCommand) = { EmptyCmd, ListCmd, NewCmd, ExitCmd, DebugCmd, RunCmd, EvalCmd, LoadCmd, SaveCmd, NodelistCmd, EditCmd, EditorCmd } ;


