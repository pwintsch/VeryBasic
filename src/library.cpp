#include "library.hpp"
#include "console.hpp"
#include "processor.hpp"
#include "error.hpp"
#include "syntax.hpp"
#include "filemgmt.hpp"
#include <cmath>
#include "bobs.hpp"






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
    MyProcessor.Reset();
    Terminal.WriteLn("All program lines and Variables cleared");
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
    int r=MyProcessor.Run(true);
    if (r==NO_ERROR) {
        return CMD_OK;
    } else {
        return r;
    }
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
    int ResultType;
    float NumResult;
    std::string StrResult;  
    int r=MyCommand.Arguments[0].Evaluate(ResultType, NumResult, StrResult);
    if (r==NO_ERROR) {
        if (ResultType==tString) {
            if (FileExists(StrResult)) {
                std::vector<std::string> lines= ReadFileLines(StrResult);
                int LinesAdded=0;
                for (int i=0; i<lines.size();i++) {
                    if (lines[i].length()>0) {
                        Instruction MyInstruction;
                        TokenCollection MyTokens;
                        int tokenizeResult=MyTokens.Tokenise(lines[i]);
                        tokenizeResult=tokenizeResult==NO_ERROR ? MyTokens.CheckBrackets():tokenizeResult;
                        tokenizeResult=tokenizeResult==NO_ERROR ? MyInstruction.Initialise(MyTokens):tokenizeResult;
                        tokenizeResult=tokenizeResult==NO_ERROR ? MyProcessor.Addline(MyInstruction):tokenizeResult;
                        if (tokenizeResult!=NO_ERROR) {
                            Terminal.WriteFStringLn("Unable to load following line:\n\r     %s", lines[i].c_str());
                            Terminal.WriteFStringLn("     %s",ErrorMsg(tokenizeResult).c_str());
                        } else {
                            LinesAdded++;   
                        }
                    }
                }
                Terminal.WriteFStringLn("Lines loaded: %d from a total of  %d", LinesAdded, lines.size());     
                return CMD_OK;
            } else {
                return ERR_FILE_NOT_FOUND;
            }
        } else {
            return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
        } 
    } else {
        return r;
    }
}


int SaveCmd(Command MyCommand)
{
    // build strng from program
    std::string s=MyProcessor.ListFull();
    // Check if file exists
    int ResultType;
    float NumResult;
    std::string StrResult;
    int r=MyCommand.Arguments[0].Evaluate(ResultType, NumResult, StrResult);
    if (r==NO_ERROR) {
        if (ResultType==tString) {
            if (FileExists(StrResult)) {
                // Ask user if they want to overwrite
                if (Terminal.GetYNConfirmation("File already exists, do you want to overwrite it ")) {
                    // overwrite file
                    WriteContentsToFile(StrResult, s);
                    Terminal.WriteLn("File overwritten");
                    return CMD_OK;
                } else {
                    Terminal.WriteLn("File not overwritten");
                    return CMD_OK;
                }                
            } else {
                // write file
                    WriteContentsToFile(StrResult, s);
                    Terminal.WriteLn("File saved");
                return CMD_OK;
            }
        } else {
            return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
        } 
    } else {
        return r;
    }
    return CMD_OK;
}

int NodelistCmd(Command MyCommand) {

    int LineToDescribe=stoi(MyCommand.Arguments[0].Value);
    Terminal.WriteFStringLn ("Full Node Description of line %d", LineToDescribe);
    std::string s="";
    int r=MyProcessor.DetailedLine(LineToDescribe, s);
    if (r==NO_ERROR) {
        Terminal.WriteLn(s.c_str());
    } else {
        return r;
    }
    return CMD_OK;
}


int EditCmd(Command MyCommand)
{
    int LineNo= stoi(MyCommand.Arguments[0].Value);
    std::string s=MyProcessor.Listline(LineNo);
    Terminal.SetNextConsoleInput(s);
    //MyProcessor.SetEditString(s);   //MyCommand.Arguments[0].GetString();
    return CMD_OK;
}

int EditorCmd(Command MyCommand)
{
    Terminal.WriteLn("EditorCmd");
    return CMD_OK;
}


int PwdCmd(Command MyCommand)
{
    std::string s;
    WorkingDirectory(s);
    Terminal.WriteFStringLn ("Current working directory: %s", s.c_str());
    return CMD_OK;
}



int ChDirCmd(Command MyCommand)
{

    int ResultType;
    float NumResult;
    std::string StrResult;
    int r=MyCommand.Arguments[0].Evaluate(ResultType, NumResult, StrResult);
    if (r==NO_ERROR) {
        if (ResultType==tString) {
            if (DirectoryExists(StrResult)) {
                ChangeCurrentDirectory(StrResult);
            } else {
                Terminal.WriteLn("Not a valid directory");
                return CMD_OK;
            }
        } else {
            return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
        } 
    } else {
        return r;
    }
    return CMD_OK;

}



int ClearCmd(Command MyCommand)
{
    MyProcessor.Clear();
    return CMD_OK;
}


int DirCmd(Command MyCommand)
{

    std::vector<std::string> FileList=DirectoryContents();
    int MaxX=Terminal.get_width();
    int MaxFileNameSize=0;
    for (int i=0; i<FileList.size(); i++) {
        if (FileList[i].length()>MaxFileNameSize) {
            MaxFileNameSize=FileList[i].length();
        }
    }
    int Width=0;
    std::string line="";
    for (int i=0; i<FileList.size(); i++) {
        Width+=MaxFileNameSize+5;
        std::string Padding="";
        for (int j=0; j<MaxFileNameSize-FileList[i].length(); j++) {
            Padding+=" ";
        }
        line=line+FileList[i]+Padding+"     ";
        if (Width+MaxFileNameSize+5>MaxX) {
            Terminal.WriteLn(line.c_str());
            line="";
            Width=0;
        }
    }
    if (line.length()>0) {
        Terminal.WriteLn(line.c_str());
    }
    return CMD_OK;
}


int ContinueCmd(Command MyCommand)
{
    int r=MyProcessor.Run(false);
    if (r==NO_ERROR) {
        return CMD_OK;
    } else {
        return r;
    }
}


int (*DirectCommandPtr[])(Command MyCommand) = { EmptyCmd, ListCmd, NewCmd, ExitCmd, DebugCmd, RunCmd, EvalCmd, LoadCmd, SaveCmd, NodelistCmd, EditCmd, EditorCmd, PwdCmd, ChDirCmd, DirCmd, ClearCmd, ContinueCmd } ;



int LetCmd(Command MyCommand) {

    int ResultType;
    float NumResult;
    std::string StrResult;  
    int r=MyCommand.Arguments[2].Evaluate(ResultType, NumResult, StrResult);
    int IntResult=(int)NumResult;
    if (r==NO_ERROR) {
        if ((MyCommand.Arguments[0].Type!=tUserDefined) || (MyCommand.Arguments[0].ID==cvString && ResultType==tValue) || (MyCommand.Arguments[0].ID!=cvString && ResultType==tString)) {
            return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
        } else {
            r=MyProcessor.SetVariable(MyCommand.Arguments[0],NumResult,StrResult);
            if (r==NO_ERROR) {
                return CMD_OK;
            } else {
                return r;
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
                    int r= MyProcessor.SetVariable(Argument,0,StrInput);
                    if (r!=NO_ERROR) {
                        return r;
                    }
                    if (i==CTRL_KEY('z')) {
                        return CMD_BREAK;
                    } else {
                        return ERR_CMD_INPUT_ERROR ;
                    }
                } else {
                    int r=MyProcessor.SetVariable(Argument,0,StrInput);
                    if (r!=NO_ERROR) {
                        return r;
                    }
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
                        int r=MyProcessor.SetVariable(Argument,0,StrInput);
                        if (r!=NO_ERROR) {
                            return r;
                        }
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
                                FltInput=(float)IntInput;
                                ValidInput=true;
                            }
                        } 
                        if (ValidInput) {
                            int r=MyProcessor.SetVariable(Argument,FltInput,StrInput);
                            if (r!=NO_ERROR) {
                                return r;
                            }
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
                } else {
                    return r;
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
    int ResultType;
    float NumResult;
    std::string StrResult;  
    int r=MyCommand.Arguments[0].Evaluate(ResultType, NumResult, StrResult); 
    if (r==NO_ERROR) {
        if (ResultType==tValue) {
            r=MyProcessor.GotoLine((int)NumResult);
            if (r==NO_ERROR) {
                return CMD_OK_POINTER_CHANGE;
            } else {
                return r;
            }
        } else {
            return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
        } 
    } else {
        return r;
    }   

}

int GosubCmd(Command MyCommand)
{
    int ResultType;
    float NumResult;
    std::string StrResult;  
    int r=MyCommand.Arguments[0].Evaluate(ResultType, NumResult, StrResult); 
    if (r==NO_ERROR) {
        if (ResultType==tValue) {
            r=MyProcessor.ReturnStack.Push(MyProcessor.CurrentLine, MyProcessor.CurrentCommand);
            r=MyProcessor.GotoLine((int)NumResult);
            if (r==NO_ERROR) {
                return CMD_OK_POINTER_CHANGE;
            } else {
                return r;
            }
        } else {
            return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
        } 
    } else {
        return r;
    }   
}

int ReturnCmd(Command MyCommand)
{
    int TmpLineIndex=0;
    int TmpCommandIndex=0;
    int r=MyProcessor.ReturnStack.Pop(TmpLineIndex, TmpCommandIndex);
    TmpCommandIndex++;
    if (r==NO_ERROR) {
        MyProcessor.CurrentLine=TmpLineIndex;
        MyProcessor.CurrentCommand=TmpCommandIndex;
        MyProcessor.ResumeInstructionFlag=true;
        return CMD_OK_POINTER_CHANGE;
    } else {
        return r;
    }
}

int StopCmd(Command MyCommand)
{
    int r=MyProcessor.Stop();
    Terminal.WriteFStringLn("PROGRAM STOPPED AT LINE %d", MyProcessor.LastLine);
    return r;
}

int ForCmd(Command MyCommand)
{
    int ResultType;
    float NumResult;
    std::string StrResult;
    int r=MyCommand.Arguments[2].Evaluate(ResultType, NumResult, StrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    double StartValue=NumResult;
    r=MyCommand.Arguments[4].Evaluate(ResultType, NumResult, StrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    double EndValue=NumResult;
    double StepValue=0;
    switch (MyCommand.RuleNo) {
        case 0:
            StepValue=1;
            break;
        case 1:
            StepValue=-1;
            break;        
        case 2:
            r=MyCommand.Arguments[6].Evaluate(ResultType, NumResult, StrResult);
            if (r!=NO_ERROR) {
                return r;
            }
            StepValue=NumResult;
            break;
        default: 
            return ERR_BAD_FOR_COMMAND;
    }
     r=MyProcessor.NewForLoop(MyCommand.Arguments[0], StartValue, EndValue, StepValue);
//    MyProcessor.ForLoopStack.Push(MyProcessor.CurrentLine, MyProcessor.CurrentCommand, MyCommand.Arguments[0].Value, StartValue, EndValue, StepValue);
    return CMD_OK;
}

int NextCmd(Command MyCommand)
{
    bool Loop;
    int NewLine;
    int NewCommand;
    int r=MyProcessor.NextForLoop(MyCommand.Arguments[0], Loop);
    if (r==NO_ERROR) {
        if (Loop) {
            MyProcessor.ResumeInstructionFlag=true;
            return CMD_OK_POINTER_CHANGE;
        } else {
            return CMD_OK;
        }
    } else {
        return r;
    }
}


int EndCmd(Command MyCommand)
{
    int r=MyProcessor.Stop();
    Terminal.WriteFStringLn("PROGRAM STOPPED AT LINE %d", MyProcessor.LastLine);
    return r;
}


int MemCmd(Command MyCommand)
{
    Terminal.WriteLn("Mem Cmd");
    return CMD_OK;
}


int DimCmd(Command MyCommand)
{

    // Check Argument 0 is a variable, which is not declared yet !!! and check it has subarguments
    // if so then check the subarguments are all expressions which evaluate to numbers and store the result in a vector of integers
    if (MyCommand.Arguments[0].Type!=tUserDefined || MyCommand.Arguments[0].SubArguments.size()==0) {
        return ERR_BAD_DIM_COMMAND;
    }
    std::vector<int> Dimensions;
    for (int i=0; i<MyCommand.Arguments[0].SubArguments.size(); i++) {
        int ResultType=0;
        float NumResult=0;
        std::string StrResult="";
        MyCommand.Arguments[0].SubArguments[i].Evaluate(ResultType, NumResult, StrResult);
        if (ResultType!=tValue) {
            return ERR_BAD_DIM_COMMAND;
        }
        int IntResult=(int)NumResult;
        Dimensions.push_back(IntResult);
    }
    int r=MyProcessor.Arrays.Create(MyCommand.Arguments[0].Value, MyCommand.Arguments[0].ID, Dimensions);
    if (r!=NO_ERROR) {
        return r;
    }
    return CMD_OK;
}


int RandomizeCmd(Command MyCommand)
{
    if (MyCommand.RuleNo==0) {
        srand(0);
    } else {
        int ResultType;
        float NumResult;
        std::string StrResult;
        int r=MyCommand.Arguments[0].Evaluate(ResultType, NumResult, StrResult);
        if (r!=NO_ERROR) {
            return r;
        }
        NumResult= NumResult<0 ? NumResult*-1 : NumResult;
        unsigned int NewSeed= (int) NumResult;
        srand(NewSeed);
    }
    return CMD_OK;
}


int OptionCmd(Command MyCommand)
{
    int ResultType;
    float NumResult;
    std::string StrResult;
    if (MyCommand.Arguments[1].ID==coInteger) {  
        int NewBase=stoi(MyCommand.Arguments[1].Value);
        if (NewBase>=0 and NewBase<2) {
            MyProcessor.Arrays.SetBase (NewBase);
            Terminal.WriteFStringLn("Array Base Option set to %d", NewBase);
        } else {
            return ERR_CMD_OPTION_BASE_OUT_OF_RANGE;
        }
    } else {
        return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
    }
    return CMD_OK;
}


int BeepCmd(Command MyCommand)
{
    Terminal.Beep();
    return CMD_OK;
}


int ClsCmd(Command MyCommand)
{
    Terminal.Clear();
    return CMD_OK;
}



int ReadCmd(Command MyCommand)
{
    int ArgNo=0;
    while (ArgNo<MyCommand.Arguments.size()) {
        if (MyCommand.Arguments[ArgNo].ID==coComma) {
            ArgNo++;
        } else {
            float NumResult;
            std::string StrResult;
            int ResultType;
            int r=MyProcessor.GetNextReadCmdData (ResultType, NumResult, StrResult);
            if (r!=NO_ERROR) {
                return r;
            }
            if ((ResultType==tValue && (MyCommand.Arguments[ArgNo].ID==cvDouble || MyCommand.Arguments[ArgNo].ID==cvInteger || MyCommand.Arguments[ArgNo].ID==cvSingle)) || (ResultType==tString && MyCommand.Arguments[ArgNo].ID==cvString)) {
                r=MyProcessor.SetVariable(MyCommand.Arguments[ArgNo],NumResult,StrResult);
                if (r!=NO_ERROR) {
                    return r;
                }
                ArgNo++;
            } else {
                return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
            }            
        }
    }
    return CMD_OK;
}   


int DataCmd(Command MyCommand)
{
    return CMD_OK;
}


int DefCmd(Command MyCommand)
{
    Terminal.WriteLn("Def Cmd");
    MyProcessor.AddDefFn(MyCommand.Arguments[0].Value, MyCommand.Arguments[0],MyCommand.Arguments[2]);
    return CMD_OK;
}


int RestoreCmd(Command MyCommand)
{
    if (MyCommand.RuleNo==0) {
        MyProcessor.ResetReadCmdData();
    } else {
        int ResultType;
        float NumResult;
        std::string StrResult;
        int r=MyCommand.Arguments[0].Evaluate(ResultType, NumResult, StrResult);
        if (r!=NO_ERROR) {
            return r;
        }
        if (ResultType==tValue) {
            r=MyProcessor.SetNextReadCmdData((int)NumResult);
            if (r!=NO_ERROR) {
                return r;
            }
        } else {
            return ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE;
        }
    }
    return CMD_OK;
}

int TmpCmd(Command MyCommand)
{
    Terminal.WriteLn("Temp Cmd");
    return CMD_OK;
}



int (*CommandPtr[])(Command MyCommand) = { LetCmd, InputCmd, RemCmd, PrintCmd, IfCmd, GotoCmd, GosubCmd, ReturnCmd, StopCmd, ForCmd, NextCmd, EndCmd, MemCmd, DimCmd, RandomizeCmd, OptionCmd, BeepCmd, ClsCmd, ReadCmd, DataCmd, RestoreCmd, DefCmd, TmpCmd } ;


int MAXFnct(CommandNode &Node,int  &ReturnType, float &NumResult, std::string &StrResult) {
    int ParamReturnType=0;
    std::string ParamStrResult="";
    float FirstParamResult=0;
    float SecondParamResult=0;
    int r=Node.SubArguments[0].Evaluate(ParamReturnType, FirstParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    r=Node.SubArguments[1].Evaluate(ParamReturnType, SecondParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    float MaxValue=FirstParamResult>SecondParamResult ? FirstParamResult : SecondParamResult;
    ReturnType=tValue;
    NumResult=MaxValue;
    return NO_ERROR;
}

int ABSFnct(CommandNode &Node,int  &ReturnType, float &NumResult, std::string &StrResult) {
    int ParamReturnType=0;
    std::string ParamStrResult="";
    float ParamResult=0;
    int r=Node.SubArguments[0].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    float AbsValue=ParamResult<0 ? -ParamResult : ParamResult;
    ReturnType=tValue;
    NumResult=AbsValue;
    return NO_ERROR;
}

int RNDFnct(CommandNode &Node,int  &ReturnType, float &NumResult, std::string &StrResult) {
    float RndValue=(float)rand()/(float)RAND_MAX;
    ReturnType=tValue;
    StrResult="";
    NumResult=RndValue;
    return NO_ERROR;
}


int SQRTFnct(CommandNode &Node,int  &ReturnType, float &NumResult, std::string &StrResult) {
    int ParamReturnType=0;
    std::string ParamStrResult="";
    float ParamResult=0;
    int r=Node.SubArguments[0].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    float SqrtValue=sqrt(ParamResult);
    ReturnType=tValue;
    StrResult="";
    NumResult=SqrtValue;
    return NO_ERROR;
}


int LEFTFnct(CommandNode &Node,int  &ReturnType, float &NumResult, std::string &StrResult) {
    int ParamReturnType=0;
    std::string ParamStrResult="";
    float ParamResult=0;
    int r=Node.SubArguments[0].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    std::string TargetStr=ParamStrResult;
    r=Node.SubArguments[1].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    int NoOfChar=int(ParamResult);
    ReturnType=tString;
    // need to truncate to right number of Char
    StrResult=TargetStr.substr(0,NoOfChar);
    return NO_ERROR;
}


int RIGHTFnct(CommandNode &Node,int  &ReturnType, float &NumResult, std::string &StrResult) {
    int ParamReturnType=0;
    std::string ParamStrResult="";
    float ParamResult=0;
    int r=Node.SubArguments[0].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    std::string TargetStr=ParamStrResult;
    r=Node.SubArguments[1].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    int NoOfChar=int(ParamResult);
    ReturnType=tString;
    // need to truncate to right number of Char
    if (NoOfChar>TargetStr.size()) {
        NoOfChar=TargetStr.size();
    }
    StrResult=TargetStr.substr(TargetStr.size()-NoOfChar, NoOfChar);
    return NO_ERROR;
}


int INKEYFnct(CommandNode &Node,int  &ReturnType, float &NumResult, std::string &StrResult) {
    StrResult="";
    NumResult=0;
    char c;
    c=Terminal.CheckKeyPress();
    if (c!='\0') {
        StrResult=c;
    }
    ReturnType=tString;
    return NO_ERROR;
}


int LENFnct(CommandNode &Node,int  &ReturnType, float &NumResult, std::string &StrResult) {
    int ParamReturnType=0;
    std::string ParamStrResult="";
    float ParamResult=0;
    int r=Node.SubArguments[0].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    ReturnType=tValue;
    NumResult=ParamStrResult.length();
    return NO_ERROR;
}  


int STRFnct(CommandNode &Node,int  &ReturnType, float &NumResult, std::string &StrResult) {
    int ParamReturnType=0;
    std::string ParamStrResult="";
    float ParamResult=0;
    int r=Node.SubArguments[0].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    std::string StrValue=std::to_string(ParamResult);
    int StringLength=StrValue.length();
    int i=0;
    for (i=StringLength-1; i>=0; i--) {
        if (StrValue[i]=='.') {
            i--;
            break;
        } else if (StrValue[i]!='0') {
            break;
        }
    }
    StrValue=StrValue.substr(0,i+1);
    ReturnType=tString;
    StrResult=StrValue;
    return NO_ERROR;
}  

bool IsStringValue (const char *sTxt) {
	if (*sTxt=='\0') {
		return false;
	}
    if (*sTxt=='+' || *sTxt=='-') {
        sTxt++;
    }
	while(*sTxt != '\0') {
		if(*sTxt < '0' || *sTxt > '9') {
			if (*sTxt!='.') {
				return false;
			}
		}		
		sTxt++;
	}
	return true;
}



int VALFnct(CommandNode &Node,int  &ReturnType, float &NumResult, std::string &StrResult) {
    int ParamReturnType=0;
    std::string ParamStrResult="";
    float ParamResult=0;
    int r=Node.SubArguments[0].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    ReturnType=tValue;
    // Convert string to number
    if (IsStringValue(ParamStrResult.c_str())) {
        NumResult=stof(ParamStrResult);
    } else {
        NumResult=0;
    }
    return NO_ERROR;
}  


int TIMERFnct(CommandNode &Node,int  &ReturnType, float &NumResult, std::string &StrResult) {

    ReturnType=tValue;
    NumResult=duration_since_midnight();
    return NO_ERROR;
}  


int INTFnct(CommandNode &Node,int  &ReturnType, float &NumResult, std::string &StrResult) {
    int ParamReturnType=0;
    std::string ParamStrResult="";
    float ParamResult=0;
    int r=Node.SubArguments[0].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    ReturnType=tValue;
    NumResult=round(ParamResult);
    return NO_ERROR;
}  


int SGNFnct(CommandNode &Node,int  &ReturnType, float &NumResult, std::string &StrResult) {
    int ParamReturnType=0;
    std::string ParamStrResult="";
    float ParamResult=0;
    int r=Node.SubArguments[0].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    float SgnValue=ParamResult<0 ? -1 : ParamResult;
    SgnValue=SgnValue==0 ? 0 : 1;
    ReturnType=tValue;
    NumResult=SgnValue;
    return NO_ERROR;
}


int MIDFnct(CommandNode &Node,int  &ReturnType, float &NumResult, std::string &StrResult) {
    int ParamReturnType=0;
    std::string ParamStrResult="";
    float ParamResult=0;
    int r=Node.SubArguments[0].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    std::string TargetStr=ParamStrResult;
    r=Node.SubArguments[1].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    int StartChar=int(ParamResult);
    if (StartChar>TargetStr.size()) {
        ReturnType=tString;
        StrResult="";
        return NO_ERROR;
    }
    r=Node.SubArguments[2].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    int NoOfChar=int(ParamResult);
    ReturnType=tString;
   
    if (NoOfChar>TargetStr.size()) {
        NoOfChar=TargetStr.size();
    }
    StrResult=TargetStr.substr(StartChar, NoOfChar);
    return NO_ERROR;
}


int INSTRFnct(CommandNode &Node,int  &ReturnType, float &NumResult, std::string &StrResult) {
    int ParamReturnType=0;
    std::string ParamStrResult="";
    float ParamResult=0;
    int r=Node.SubArguments[0].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    std::string SourceStr=ParamStrResult;
    r=Node.SubArguments[1].Evaluate(ParamReturnType, ParamResult, ParamStrResult);
    if (r!=NO_ERROR) {
        return r;
    }
    std::string TargetStr=ParamStrResult;

    std::size_t found = SourceStr.find(TargetStr);
    if (found!=std::string::npos) {
        ReturnType=tValue;
        NumResult=found+1;
    } else {
        ReturnType=tValue;
        NumResult=0;
    }
    ReturnType=tValue;
    return NO_ERROR;
}


int (*FunctionPtr[])(CommandNode &Node, int &ReturnType, float &NumResult, std::string &StrResult) = { ABSFnct, RNDFnct, MAXFnct, SQRTFnct, LEFTFnct, INKEYFnct, LENFnct, STRFnct, VALFnct, TIMERFnct, INTFnct, RIGHTFnct, SGNFnct, MIDFnct, INSTRFnct };
