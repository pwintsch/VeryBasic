#include "library.hpp"
#include "console.hpp"

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

int EmptyCmd()
{
    Terminal.WriteLn("EmptyCmd");
    return 0;
}


int ListCmd()
{
    Terminal.WriteLn("ListCmd");
    return 0;
}

int NewCmd()
{
    Terminal.WriteLn("NewCmd");
    return 0;
}

int ExitCmd()
{
    Terminal.WriteLn("ExitCmd");
    return 0;
}


int DebugCmd()
{
    Terminal.WriteLn("DebugCmd");
    return 0;
}


int RunCmd()
{
    Terminal.WriteLn("RunCmd");
    return 0;
}


int EvalCmd()
{
    Terminal.WriteLn("EvalCmd");
    return 0;
}


int LoadCmd()
{
    Terminal.WriteLn("LoadCmd");
    return 0;
}


int SaveCmd()
{
    Terminal.WriteLn("SaveCmd");
    return 0;
}

int NodelistCmd()
{
    Terminal.WriteLn("NodelistCmd");
    return 0;
}

int EditCmd()
{
    Terminal.WriteLn("EditCmd");
    return 0;
}

int EditorCmd()
{
    Terminal.WriteLn("EditorCmd");
    return 0;
}


int (*DirectCommandPtr[])() = { EmptyCmd, ListCmd, NewCmd, ExitCmd, DebugCmd, RunCmd, EvalCmd, LoadCmd, SaveCmd, NodelistCmd, EditCmd, EditorCmd } ;


