
#include <iostream>
#include <string>
#include "console.h"

#define MAX_STRING_LENGTH 255



void PromptLoop(Console MyConsole) {
char sConsoleInput[255];
bool bMachineLoop;
	bMachineLoop=true;
	while (bMachineLoop) {
		strncpy(sConsoleInput, "",2);
		while (strlen(sConsoleInput)==0) {
			MyConsole.GetConsoleInput (sConsoleInput, MAX_STRING_LENGTH);
		}
        if (sConsoleInput[0]=='q') {
            MyConsole.WriteLn("Ending ...");
            bMachineLoop=false;
        } else {
            MyConsole.WriteLn("OK");
        }
    }
}


int main() {
    Console console;
    console.clear();
    console.WriteLn("Hello, world!");
    console.WriteLn("I am using my new console class! v0.0004");
    console.WriteFStringLn ("Terminal Size: Rows: %d Columns: %d ", console.get_height(), console.get_width());
    console.WriteLn("");
    PromptLoop(console);
    return 0;
}
