

#include "console.hpp"
#include "token.hpp"
#include "syntax.hpp"
#include "error.hpp"
#include "instruction.hpp"
#include "library.hpp"

#include <string>
#define MAX_STRING_LENGTH 255

std::vector<Instruction> Program;


void PromptLoop() {
std::string sInput;
bool bMachineLoop = true;
    // temp message whilst building
    Terminal.WriteLn("Enter q to quit");

    Program.clear();
	while (bMachineLoop) {
        sInput="";
        while (sInput=="") {
            Terminal.GetConsoleInput (sInput, MAX_STRING_LENGTH);
    		}
        if (sInput=="?") {
            Terminal.WriteLn("Help");
		}
        if (sInput=="q") {
            Terminal.WriteLn("Ending ...");
            int r=functptr[2]();
            Terminal.WriteFStringLn("Result: %d", r);
            bMachineLoop=false;
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
                int i;
                if (Program.size()==0) {
                    Program.push_back(MyInstruction);
                } else {
                    for (i=0; i<Program.size(); i++) {
                        // if programline is equal replace instruction
                        if (Program[i].ProgramLine == MyInstruction.ProgramLine) {
                            Program[i]=MyInstruction;
                            break;
                        }
                        if (Program[i].ProgramLine > MyInstruction.ProgramLine) {
                            break;
                        }
                    }
                    Program.insert(Program.begin()+i, MyInstruction);
                }
                Terminal.WriteLn(MyInstruction.GetString().c_str());
            }
            Terminal.WriteLn("OK");
        }
    }
}
 

int main() {
    
    Terminal.Initialise();
    Terminal.clear();
    Terminal.WriteLn("Hello, world!");
    Terminal.WriteLn("I am using my new console class! v0.0009a");
    Terminal.WriteFStringLn ("Terminal Size: Rows: %d Columns: %d ", Terminal.get_height(), Terminal.get_width());
    Terminal.WriteLn("");
    PromptLoop();
    return 0;
}
