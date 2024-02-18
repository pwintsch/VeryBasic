#include "console.hpp"
#include "token.hpp"
#include "syntax.hpp"
#include "error.hpp"
#include "instruction.hpp"
#include "library.hpp"
#include "command.hpp"
#include "test.hpp"
#include "processor.hpp"
#include <string>

#define MAX_STRING_LENGTH 255

bool TestOn = false;

std::vector<Instruction> Program;


void PromptLoop() {
std::string sInput;
bool bMachineLoop = true;
 
    if (TestOn) {
        Terminal.WriteLn("Test mode on");
        Terminal.WriteLn("Running Parsing and Lexing tests");
        int i;
        bool TestRun=true;
        for (i=0; i<TestStatements.size(); i++) {
            Terminal.WriteFString("Test %d :: ", i);
            TokenCollection MyTokens;
            int tokenizeResult=MyTokens.Tokenise(TestStatements[i].Statement);
            tokenizeResult=tokenizeResult==NO_ERROR ? MyTokens.CheckBrackets():tokenizeResult;
            Instruction MyInstruction;
            tokenizeResult=tokenizeResult==NO_ERROR ? MyInstruction.Initialise(MyTokens):tokenizeResult;
            if (tokenizeResult!=NO_ERROR) {
                Terminal.WriteLn(ErrorMsg(tokenizeResult).c_str());
            } else {
                Terminal.Write(MyInstruction.GetString().c_str());
                Terminal.Write(" :: ");
                int r=MyInstruction.NodeCount();
                if (r!=TestStatements[i].NodeCount) {
                    TestRun=false;
                    Terminal.WriteFStringLn("Node count error: result was %d, expecting %d", r, TestStatements[i].NodeCount);
                } else {
                    Terminal.WriteFStringLn("Node count: %d", r);
                }               
            }
        }
        Terminal.WriteLn("Tests complete");
        if (TestRun) {
            Terminal.WriteLn("All tests passed");
        } else {
            Terminal.WriteLn("TESTS FAILED");
        }
        Terminal.WriteLn("OK");
    }
    // temp message whilst building
    Terminal.WriteLn("Enter q to quit");
    Program.clear();
	while (MyProcessor.Active==true) {
        sInput="";
        while (sInput=="") {
/*            if (MyProcessor.GetEditString()!="") {
                sInput=MyProcessor.GetEditString();
                MyProcessor.ResetEditString();
            } else { 
                sInput="";
            }*/
            Terminal.GetConsoleInput (sInput, MAX_STRING_LENGTH);
    	}
        if (sInput=="?") {
            Terminal.WriteLn("Help");
		}
        if (sInput=="q") {
            Terminal.WriteLn("Ending ...");
            MyProcessor.Exit();
        } else {
            TokenCollection MyTokens;
            int tokenizeResult=MyTokens.Tokenise(sInput);
            tokenizeResult=tokenizeResult==NO_ERROR ? MyTokens.CheckBrackets():tokenizeResult;
            Instruction MyInstruction;
            tokenizeResult=tokenizeResult==NO_ERROR ? MyInstruction.Initialise(MyTokens):tokenizeResult;
            if (tokenizeResult!=NO_ERROR) {
                Terminal.WriteLn(ErrorMsg(tokenizeResult).c_str());
            } else {
                // add instruction to program in order
                if (MyInstruction.ProgramLine==0) {
                    bool ConditionFailed=false;
                    if (MyInstruction.Commands[0].Type==tDirectCommand) {
                        bool NoBreakOrError=true;
                        int r=DirectCommandPtr[(MyInstruction.Commands[0].ID-DirectCmdSep)](MyInstruction.Commands[0]);
                        if (r<CMD_OK) {
                            Terminal.WriteLn("");
                            Terminal.WriteFStringLn("Error at Line %d", MyProcessor.LastProgramLine());
                            Terminal.WriteLn(ErrorMsg(r).c_str());
                            NoBreakOrError=false;
                        } else {
                            switch (r) {
                                case CMD_OK:
                                    break;
                                case CMD_OK_Cond_Fail:
                                    ConditionFailed=true;
                                    break;
                                case CMD_STOP:
                                    Terminal.WriteLn("STOP");
                                    NoBreakOrError=false;
                                    break;
                                case CMD_BREAK:
                                    Terminal.WriteLn("BREAK Requested by user");
                                    NoBreakOrError=false;
                                    break;
                                case CMD_ENDRUN:
                                    Terminal.WriteLn("END RUN");
                                    NoBreakOrError=false;
                                    break;
                                default:
                                    Terminal.WriteLn("Unknown command return code");
                                    NoBreakOrError=false;
                                    break;
                            }
                        }
                    } else {
                        int r=NO_ERROR;
                        bool NoBreakOrError=true;
                        int i=0;
                        while (NoBreakOrError && i<MyInstruction.Commands.size() && !ConditionFailed) {                      
                            if (MyInstruction.Commands[i].Type==tUserDefined) {
                                r=LetCmd(MyInstruction.Commands[i]);
                            } else { 
                                r=CommandPtr[(MyInstruction.Commands[i].ID-CmdSep)](MyInstruction.Commands[i]);
                            }
                            if (r<CMD_OK) {
                                Terminal.WriteLn("");
                                Terminal.WriteLn(ErrorMsg(r).c_str());
                                NoBreakOrError=false;
                            } else {
                                switch (r) {
                                    case CMD_OK:
                                        break;
                                    case CMD_OK_Cond_Fail:
                                        ConditionFailed=true;
                                        break;
                                    case CMD_STOP:
                                        Terminal.WriteLn("STOP");
                                        NoBreakOrError=false;
                                        break;
                                    case CMD_BREAK:
                                        Terminal.WriteLn("BREAK Requested by user");
                                        NoBreakOrError=false;
                                        break;
                                    case CMD_ENDRUN:
                                        Terminal.WriteLn("END RUN");
                                        NoBreakOrError=false;
                                        break;
                                    default:
                                        Terminal.WriteLn("Unknown command return code");
                                        NoBreakOrError=false;
                                        break;
                                }
                            }
                            i++;
                        }
                    }
                } else {
                    int r=MyProcessor.Addline(MyInstruction);
                    if (r==ERR_LINE_ALREADY_EXISTS) {
                        std::string s="Line already exists, do you want to replace it?";
                        bool ChangeLine=Terminal.GetYNConfirmation(s.c_str());
                        if (ChangeLine) {
                            r=MyProcessor.ChangeLine(MyInstruction);
                        }
                    }
                }
            }
            Terminal.WriteLn("OK");
        }
    }
}
 

int main() {
    
    Terminal.Initialise();
    Terminal.Clear();
    Terminal.WriteLn("Hello, world!");
    Terminal.WriteLn("I am using my new console class! v0.0010c");
    Terminal.WriteFStringLn ("Terminal Size: Rows: %d Columns: %d ", Terminal.get_height(), Terminal.get_width());
    Terminal.WriteLn("");
    PromptLoop();
    return 0;
}
