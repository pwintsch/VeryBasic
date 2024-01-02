#include "library.hpp"
#include "console.hpp"
#include "processor.hpp"
#include "error.hpp"
#include "syntax.hpp"





int EmptyCmd(Command MyCommand)
{
    Terminal.WriteLn("EmptyCmd");
    return CMD_OK;
}


int ListCmd(Command MyCommand)
{   
    if (MyCommand.RuleNo==0) {
        std::string s=MyProcessor.ListFull();
        Terminal.WriteLn(s.c_str());
        return CMD_OK;
    } else if (MyCommand.RuleNo==1) {
        int LineNo= stoi(MyCommand.Arguments[0].Value);
        std::string s=MyProcessor.Listline(LineNo);
        Terminal.WriteLn(s.c_str());
        return CMD_OK;
    } else if (MyCommand.RuleNo==2) {
        int StartLineNo= stoi(MyCommand.Arguments[0].Value);
        int EndLineNo= stoi(MyCommand.Arguments[2].Value);
        std::string s=MyProcessor.ListPartial(StartLineNo, EndLineNo);
        Terminal.WriteLn(s.c_str());
        return CMD_OK;
    } else {
        return ERR_SYNTAX;
    }
}

int NewCmd(Command MyCommand)
{
    Terminal.WriteLn("NewCmd");
    return CMD_OK;
}

int ExitCmd(Command MyCommand)
{
    MyProcessor.Exit();
    return CMD_OK;
}


int DebugCmd(Command MyCommand)
{
    Terminal.WriteLn("DebugCmd");
    return CMD_OK;
}


int RunCmd(Command MyCommand)
{
    return MyProcessor.Run();
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
        return CMD_OK;
    }
    
    return r;
}


int LoadCmd(Command MyCommand)
{
    Terminal.WriteLn("LoadCmd");
    return CMD_OK;
}


int SaveCmd(Command MyCommand)
{
    Terminal.WriteLn("SaveCmd");
    return CMD_OK;
}

int NodelistCmd(Command MyCommand)
{
    Terminal.WriteLn("NodelistCmd");
    return CMD_OK;
}

int EditCmd(Command MyCommand)
{
    Terminal.WriteLn("EditCmd");
    return CMD_OK;
}

int EditorCmd(Command MyCommand)
{
    Terminal.WriteLn("EditorCmd");
    return CMD_OK;
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
                        return CMD_OK;
                        break;
                    case cvSingle:
                    case cvDouble:
                        MyProcessor.Variables.Store(MyCommand.Arguments[0].Value, MyCommand.Arguments[0].ID, NumResult, 0, "");
                        return CMD_OK;
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
                return CMD_OK;
            } else {
                return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
            }
        } 
    }  
    return r;
}


int InputCmd(Command MyCommand)
{
    int ResultType;
    float NumResult;
    std::string StrResult;
    bool LastExpressionWasVariable=true;
    int MaxX=Terminal.get_width();
    int MaxY=Terminal.get_height();
    if (MyCommand.Arguments.size()>0 && MyCommand.Arguments[0].SubArguments.size()>0) {
        for (auto &Argument : MyCommand.Arguments[0].SubArguments) {
            if (Argument.ID==cvString) {
                std::string StrInput="";
                int i=Terminal.GetString(StrInput);
                if (i==0 || i==CTRL_KEY('z')) {
                    StrInput="";
                    MyProcessor.Variables.Store(Argument.Value, cvString, 0, 0, StrInput);
                    if (i==CTRL_KEY('z')) {
                        return CMD_BREAK;
                    } else {
                        return ERR_CMD_INPUT_ERROR ;
                    }
                } else {
                    MyProcessor.Variables.Store(Argument.Value, cvString, 0, 0, StrInput);
                }
            } else if (Argument.ID==cvDouble || Argument.ID==cvSingle || Argument.ID==cvInteger) {
                LastExpressionWasVariable=true;    
                std::string StrInput="";
                bool ValidInput=false;
                while (!ValidInput) {
                    int CurrentX=0;
                    int CurrentY=0;
                    Terminal.GetCursorPos (CurrentY, CurrentX);
                    int i=Terminal.GetString(StrInput);
                    if (i==0 || i==CTRL_KEY('z')) {
                        StrInput="";
                        MyProcessor.Variables.Store(Argument.Value, Argument.ID, 0, 0, StrInput);
                        if (i==CTRL_KEY('z')) {
                            return CMD_BREAK;
                        } else {
                            return ERR_CMD_INPUT_ERROR ;
                        }
                    } else {
                        float FltInput=0;
                        int IntInput=0;
                        if (Argument.ID==cvDouble || Argument.ID==cvSingle) {
                            if (IsStringFloat(StrInput.c_str())) {
                                FltInput=stof(StrInput);
                                ValidInput=true;
                            }
                        } else {
                            if (IsStringInt(StrInput.c_str())) {
                                IntInput=stoi(StrInput);
                                ValidInput=true;
                            }
                        } 
                        if (ValidInput) {
                            MyProcessor.Variables.Store(Argument.Value, Argument.ID, FltInput, IntInput, StrInput);
                        } else {
                            Terminal.Beep();
                            Terminal.MoveCursor(CurrentY, CurrentX);
                            std::string s="";
                            for (int i=0; i<StrInput.length(); i++) {
                                s+=" ";
                            }
                            Terminal.Write(s.c_str());
                            Terminal.MoveCursor(CurrentY, CurrentX);
                        }
                    }
                }
            } else if (Argument.Type==tExpression) {
                LastExpressionWasVariable=false;
                int r=Argument.Evaluate(ResultType, NumResult, StrResult);
                int IntResult=(int)NumResult;
                if (r==NO_ERROR) {
                    if (ResultType==tValue) {
                        Terminal.WriteFString("%f", NumResult);
                    } else {
                        Terminal.WriteFString("%s", StrResult.c_str());
                    } 
                }  
            } else {
                LastExpressionWasVariable=false;
                int r=0;
                int ATLine=0;
                int ATColumn=0;
                switch (Argument.ID) {
                    case coString:
                    case coInteger:
                    case coDouble:                   
                        Terminal.WriteFString("%s", Argument.Value.c_str());
                        break;
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
                        r=Argument.SubArguments[0].Evaluate(ResultType, NumResult, StrResult);
                        if (r==NO_ERROR) {
                            if (ResultType==tValue) {
                                Terminal.MoveCursorToColumn((int)NumResult);
                            } else {
                                return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
                            } 
                        } else {
                            return r;
                        }
                        break;
                    case coAT:
                        ATLine=0;
                        ATColumn=0;
                        r=Argument.SubArguments[0].Evaluate(ResultType, NumResult, StrResult);
                        if (r==NO_ERROR) {
                            if (ResultType==tValue) {
                                ATLine=(int)NumResult;
                                r=Argument.SubArguments[2].Evaluate(ResultType, NumResult, StrResult);
                                if (ResultType==tValue) {
                                    ATColumn=(int)NumResult;
                                    if (ATLine<0 || ATLine>=MaxY || ATColumn<0 || ATColumn>=MaxX) {
                                        return ERR_BAD_PRINT_COORDINATES;
                                    } else {
                                        Terminal.MoveCursor(ATLine, ATColumn);
                                    }
                                } else {
                                    return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
                                }
                            } else {
                                return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
                            } 
                        } else {
                            return r;
                        }
                        break;
                    default:
                        return ERR_BAD_INPUT_EXPRESSION;
                        break;
                } 
            }
        }
    }
    if (LastExpressionWasVariable) {
        Terminal.WriteLn("");
    }
    return CMD_OK;

}


int RemCmd(Command MyCommand)
{
    Terminal.WriteLn("Rem Cmd");
    return CMD_OK;
}


int PrintCmd(Command MyCommand) {
    int ResultType;
    float NumResult;
    std::string StrResult;
    bool LastExpressionWasExpression=true;
    int MaxX=Terminal.get_width();
    int MaxY=Terminal.get_height();
    if (MyCommand.Arguments.size()>0 && MyCommand.Arguments[0].SubArguments.size()>0) {
        for (auto &Argument : MyCommand.Arguments[0].SubArguments) {
            if (Argument.Type==tExpression) {
                LastExpressionWasExpression=true;
                int r=Argument.Evaluate(ResultType, NumResult, StrResult);
                int IntResult=(int)NumResult;
                if (r==NO_ERROR) {
                    if (ResultType==tValue) {
                        Terminal.WriteFString("%f", NumResult);
                    } else {
                        Terminal.WriteFString("%s", StrResult.c_str());
                    } 
                }  
            } else {
                LastExpressionWasExpression=false;
                int r=0;
                int ATLine=0;
                int ATColumn=0;
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
                        r=Argument.SubArguments[0].Evaluate(ResultType, NumResult, StrResult);
                        if (r==NO_ERROR) {
                            if (ResultType==tValue) {
                                Terminal.MoveCursorToColumn((int)NumResult);
                            } else {
                                return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
                            } 
                        } else {
                            return r;
                        }
                        break;
                    case coAT:
                        ATLine=0;
                        ATColumn=0;
                        r=Argument.SubArguments[0].Evaluate(ResultType, NumResult, StrResult);
                        if (r==NO_ERROR) {
                            if (ResultType==tValue) {
                                ATLine=(int)NumResult;
                                r=Argument.SubArguments[2].Evaluate(ResultType, NumResult, StrResult);
                                if (ResultType==tValue) {
                                    ATColumn=(int)NumResult;
                                    if (ATLine<0 || ATLine>=MaxY || ATColumn<0 || ATColumn>=MaxX) {
                                        return ERR_BAD_PRINT_COORDINATES;
                                    } else {
                                        Terminal.MoveCursor(ATLine, ATColumn);
                                    }
                                } else {
                                    return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
                                }
                            } else {
                                return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
                            } 
                        } else {
                            return r;
                        }
                        break;
                    default:
                        return ERR_BAD_PRINT_EXPRESSION;
                        break;
                } 
            }
        }
    }
    if (LastExpressionWasExpression) {
        Terminal.WriteLn("");
    }
    return CMD_OK;
}


int IfCmd(Command MyCommand)
{

    int ResultType;
    float NumResult;
    std::string StrResult;  
    int r=MyCommand.Arguments[0].Evaluate(ResultType, NumResult, StrResult);
    if (r==NO_ERROR) {
        if (ResultType==tValue) {
            if (NumResult!=0) {
                return CMD_OK;
            } else {
                return CMD_OK_Cond_Fail;
            }
        } else {
            return ERR_CMD_IF_EXPRESSION_NOT_VALUE;
        } 
    } else {
        return r;
    }
}


int GotoCmd(Command MyCommand)
{
    Terminal.WriteLn("Goto Cmd");
    return CMD_OK;
}

int GosubCmd(Command MyCommand)
{
    Terminal.WriteLn("Gosub Cmd");
    return CMD_OK;
}

int ReturnCmd(Command MyCommand)
{
    Terminal.WriteLn("Return Cmd");
    return CMD_OK;
}

int StopCmd(Command MyCommand)
{
    Terminal.WriteLn("Stop Cmd");
    return CMD_OK;
}

int ForCmd(Command MyCommand)
{
    Terminal.WriteLn("For Cmd");
    return CMD_OK;
}

int NextCmd(Command MyCommand)
{
    Terminal.WriteLn("Next Cmd");
    return CMD_OK;
}

int ClearCmd(Command MyCommand)
{
    Terminal.WriteLn("Clear Cmd");
    return CMD_OK;
}

int EndCmd(Command MyCommand)
{
    Terminal.WriteLn("End Cmd");
    return CMD_OK;
}

int MemCmd(Command MyCommand)
{
    Terminal.WriteLn("Mem Cmd");
    return CMD_OK;
}

int DimCmd(Command MyCommand)
{
    Terminal.WriteLn("Dim Cmd");
    return CMD_OK;
}


int RandomizeCmd(Command MyCommand)
{
    Terminal.WriteLn("Randomize Cmd");
    return CMD_OK;
}


int OptionCmd(Command MyCommand)
{
    Terminal.WriteLn("Option Cmd");
    return CMD_OK;
}


int ContinueCmd(Command MyCommand)
{
    Terminal.WriteLn("Continue Cmd");
    return CMD_OK;
}


int BeepCmd(Command MyCommand)
{
    Terminal.Beep();
    return CMD_OK;
}


int ClsCmd(Command MyCommand)
{
    Terminal.WriteLn("Cls Cmd");
    return CMD_OK;
}



int ReadCmd(Command MyCommand)
{
    Terminal.WriteLn("Read Cmd");
    return CMD_OK;
}   


int DataCmd(Command MyCommand)
{
    Terminal.WriteLn("Data Cmd");
    return CMD_OK;
}


int DefCmd(Command MyCommand)
{
    Terminal.WriteLn("Def Cmd");
    return CMD_OK;
}


int TmpCmd(Command MyCommand)
{
    Terminal.WriteLn("Tmp Cmd");
    return CMD_OK;
}


int (*CommandPtr[])(Command MyCommand) = { LetCmd, InputCmd, RemCmd, PrintCmd, IfCmd, GotoCmd, GosubCmd, ReturnCmd, StopCmd, ForCmd, NextCmd, ClearCmd, EndCmd, MemCmd, DimCmd, RandomizeCmd, OptionCmd, ContinueCmd, BeepCmd, ClsCmd, ReadCmd, DataCmd, DefCmd, TmpCmd } ;
