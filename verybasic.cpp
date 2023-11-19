

#include "console.h"
#include "token.h"
#include "syntax.h"
#include "error.hpp"
#include <string>
#define MAX_STRING_LENGTH 255



void PromptLoop(Console MyConsole) {
std::string sInput;
bool bMachineLoop = true;
    // temp message whilst building
    MyConsole.WriteLn("Enter q to quit");

	while (bMachineLoop) {
        sInput="";
        while (sInput=="") {
            MyConsole.GetConsoleInput (sInput, MAX_STRING_LENGTH);
    		}
        if (sInput=="?") {
            MyConsole.WriteLn("Help");
		}
        if (sInput=="q") {
            MyConsole.WriteLn("Ending ...");
            bMachineLoop=false;
        } else {
            MyConsole.WriteLn("You entered: ");
            MyConsole.WriteLn(sInput.c_str());
            TokenCollection MyTokens;
            int tokenizeResult=MyTokens.Tokenise(sInput);
            if (tokenizeResult==NO_ERROR) {
                tokenizeResult=MyTokens.CheckBrackets();
            }
            if (tokenizeResult!=NO_ERROR) {
                MyConsole.WriteLn(ErrorMsg(tokenizeResult).c_str());
            } else {
                MyConsole.WriteLn("Tokenise & Bracket Check OK");
            }
            MyConsole.WriteLn(MyTokens.GetString().c_str());

            MyConsole.WriteLn("OK");
        }
    }
}
 

int main() {
    Console console;
    console.clear();
    console.WriteLn("Hello, world!");
    console.WriteLn("I am using my new console class! v0.0009a");
    console.WriteFStringLn ("Terminal Size: Rows: %d Columns: %d ", console.get_height(), console.get_width());
    console.WriteLn("");
    PromptLoop(console);
    return 0;
}
