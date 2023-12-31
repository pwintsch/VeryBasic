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
	while (MyProcessor.Active) {
        sInput="";
        while (sInput=="") {
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
                    if (MyInstruction.Commands[0].Type==tDirectCommand) {
                        int r=DirectCommandPtr[(MyInstruction.Commands[0].ID-DirectCmdSep)](MyInstruction.Commands[0]);
                        if (r!=NO_ERROR) {
                            Terminal.WriteLn(ErrorMsg(r).c_str());
                        } else {
                            Terminal.WriteLn("OK");
                        }
                    } else {
                        int r=NO_ERROR;
                        if (MyInstruction.Commands[0].Type==tUserDefined) {
                            r=LetCmd(MyInstruction.Commands[0]);
                        } else { 
                            r=CommandPtr[(MyInstruction.Commands[0].ID-CmdSep)](MyInstruction.Commands[0]);
                        }
                        if (r!=NO_ERROR) {
                            Terminal.WriteLn(ErrorMsg(r).c_str());
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
//                    Terminal.WriteLn(MyInstruction.GetString().c_str());
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
    Terminal.WriteLn("I am using my new console class! v0.0009a");
    Terminal.WriteFStringLn ("Terminal Size: Rows: %d Columns: %d ", Terminal.get_height(), Terminal.get_width());
    Terminal.WriteLn("");
    PromptLoop();
    return 0;
}
