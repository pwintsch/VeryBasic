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
    int ResultType;
    float NumResult;
    std::string StrResult;  
    int r=MyCommand.Arguments[0].Evaluate(ResultType, NumResult, StrResult);
    if (r==NO_ERROR) {
        if (ResultType==tValue) {
            Terminal.WriteFStringLn("Result: %f", NumResult);           
        } else {
            Terminal.WriteFStringLn("Result: %s", StrResult.c_str());
        } 
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



int LetCmd(Command MyCommand)
{

    int ResultType;
    float NumResult;
    std::string StrResult;  
    int r=MyCommand.Arguments[2].Evaluate(ResultType, NumResult, StrResult);
    int IntResult=(int)NumResult;
    if (r==NO_ERROR) {
        if (ResultType==tValue) {
            if (MyCommand.Arguments[0].ID!=cvString) {
                switch (MyCommand.Arguments[0].ID) {
                    case cvInteger:
                        MyProcessor.Variables.Store(MyCommand.Arguments[0].Value, MyCommand.Arguments[0].ID, IntResult, 0, "");
                        break;
                    case cvSingle:
                    case cvDouble:
                        MyProcessor.Variables.Store(MyCommand.Arguments[0].Value, MyCommand.Arguments[0].ID, NumResult, 0, "");
                        break;
                    default:
                        return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
                }
            } else {
                return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
            }        
        } else {
            if (MyCommand.Arguments[0].ID==cvString) {
                MyProcessor.Variables.Store(MyCommand.Arguments[0].Value, cvString, 0, 0, StrResult);
            } else {
                return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
            }
        } 
    }  
    return r;
}


int InputCmd(Command MyCommand)
{
    Terminal.WriteLn("Input Cmd");
    return NO_ERROR;
}


int RemCmd(Command MyCommand)
{
    Terminal.WriteLn("Rem Cmd");
    return NO_ERROR;
}


int PrintCmd(Command MyCommand) {
    int ResultType;
    float NumResult;
    std::string StrResult;  
    if (MyCommand.Arguments[0].SubArguments.size()>0) {
        for (auto &Argument : MyCommand.Arguments[0].SubArguments) {
            if (Argument.Type==tExpression) {
                int r=Argument.Evaluate(ResultType, NumResult, StrResult);
                int IntResult=(int)NumResult;
                if (r==NO_ERROR) {
                    if (ResultType==tValue) {
                        Terminal.WriteFString("%f ", NumResult);
                    } else {
                        Terminal.WriteFString("%s ", StrResult.c_str());
                    } 
                }  
            } else {
                int r=0;
                switch (Argument.ID) {
                    case coComma:
                        Terminal.MoveCursorToNextTab();
                        break;
                    case coSemiColon:
                        Terminal.Write("");
                        break;
                    case coExclamation:
                        Terminal.Write(" ");
                        break;
                    case coBackSlash:
                        Terminal.WriteLn("");
                        break;
                    case coTAB:
                        Terminal.MoveCursorToNextTab();
                        r=Argument.SubArguments[0].Evaluate(ResultType, NumResult, StrResult);
                        if (r==NO_ERROR) {
                            if (ResultType==tValue) {
                                Terminal.WriteFStringLn("Moving To Column %d", NumResult);
//                                int NumResult=(int)NumResult;
//                                Terminal.MoveCursorToColumn(NumResult);
                            } else {
                                return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
                            } 
                        } else {
                            return r;
                        }
                        Terminal.WriteFStringLn("Moving To Column %d", NumResult);
                        break;
                    default:
                        return ERR_SYNTAX;
                        break;
                } 
            }
        }
    }
    return NO_ERROR;
}


int IfCmd(Command MyCommand)
{
    Terminal.WriteLn("If Cmd");
    return NO_ERROR;
}


int GotoCmd(Command MyCommand)
{
    Terminal.WriteLn("Goto Cmd");
    return NO_ERROR;
}

int GosubCmd(Command MyCommand)
{
    Terminal.WriteLn("Gosub Cmd");
    return NO_ERROR;
}

int ReturnCmd(Command MyCommand)
{
    Terminal.WriteLn("Return Cmd");
    return NO_ERROR;
}

int StopCmd(Command MyCommand)
{
    Terminal.WriteLn("Stop Cmd");
    return NO_ERROR;
}

int ForCmd(Command MyCommand)
{
    Terminal.WriteLn("For Cmd");
    return NO_ERROR;
}

int NextCmd(Command MyCommand)
{
    Terminal.WriteLn("Next Cmd");
    return NO_ERROR;
}

int ClearCmd(Command MyCommand)
{
    Terminal.WriteLn("Clear Cmd");
    return NO_ERROR;
}

int EndCmd(Command MyCommand)
{
    Terminal.WriteLn("End Cmd");
    return NO_ERROR;
}

int MemCmd(Command MyCommand)
{
    Terminal.WriteLn("Mem Cmd");
    return NO_ERROR;
}

int DimCmd(Command MyCommand)
{
    Terminal.WriteLn("Dim Cmd");
    return NO_ERROR;
}


int RandomizeCmd(Command MyCommand)
{
    Terminal.WriteLn("Randomize Cmd");
    return NO_ERROR;
}


int OptionCmd(Command MyCommand)
{
    Terminal.WriteLn("Option Cmd");
    return NO_ERROR;
}


int ContinueCmd(Command MyCommand)
{
    Terminal.WriteLn("Continue Cmd");
    return NO_ERROR;
}


int BeepCmd(Command MyCommand)
{
    Terminal.WriteLn("Beep Cmd");
    return NO_ERROR;
}


int ClsCmd(Command MyCommand)
{
    Terminal.WriteLn("Cls Cmd");
    return NO_ERROR;
}



int ReadCmd(Command MyCommand)
{
    Terminal.WriteLn("Read Cmd");
    return NO_ERROR;
}   


int DataCmd(Command MyCommand)
{
    Terminal.WriteLn("Data Cmd");
    return NO_ERROR;
}


int DefCmd(Command MyCommand)
{
    Terminal.WriteLn("Def Cmd");
    return NO_ERROR;
}


int TmpCmd(Command MyCommand)
{
    Terminal.WriteLn("Tmp Cmd");
    return NO_ERROR;
}


int (*CommandPtr[])(Command MyCommand) = { LetCmd, InputCmd, RemCmd, PrintCmd, IfCmd, GotoCmd, GosubCmd, ReturnCmd, StopCmd, ForCmd, NextCmd, ClearCmd, EndCmd, MemCmd, DimCmd, RandomizeCmd, OptionCmd, ContinueCmd, BeepCmd, ClsCmd, ReadCmd, DataCmd, DefCmd, TmpCmd } ;
