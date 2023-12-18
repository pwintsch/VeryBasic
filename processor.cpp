#include "processor.hpp"
#include "syntax.hpp"
#include "error.hpp"

Processor MyProcessor;

Processor::Processor() {
}

Processor::~Processor() {
}


int Processor::Addline(Instruction MyInstruction) {
    int i;
    if (Program.size()==0) {
        Program.push_back(MyInstruction);
        return NO_ERROR;
    } else {
        for (i=0; i<Program.size(); i++) {
            // if programline is equal replace instruction
            if (Program[i].ProgramLine == MyInstruction.ProgramLine) {
                return ERR_LINE_ALREADY_EXISTS;
            } else if (Program[i].ProgramLine > MyInstruction.ProgramLine) {
                Program.insert(Program.begin()+i, MyInstruction);
                return NO_ERROR;
            }
        }
    }
    Program.push_back(MyInstruction);
    return NO_ERROR;
}

int Processor::ChangeLine(Instruction MyInstruction) {
    int i;
    if (Program.size()==0) {
        Program.push_back(MyInstruction);
    } else {
        for (i=0; i<Program.size(); i++) {
            // if programline is equal replace instruction
            if (Program[i].ProgramLine == MyInstruction.ProgramLine) {
                Program[i]=MyInstruction;
                return NO_ERROR;
            }
        }
    }
    return ERR_NO_LINE_NUMBER;
}

std::string Processor::ListFull() {
    std::string s="";
    for (int i=0; i<Program.size(); i++) {
        s+=Program[i].GetString()+"\n\r";
    }
    return s;
}

std::string Processor::ListPartial(int StartLineNo, int EndLineNo) {
    std::string s="";
    for (int i=0; i<Program.size(); i++) {
        if (Program[i].ProgramLine>=StartLineNo && Program[i].ProgramLine<=EndLineNo) {
            s+=Program[i].GetString()+"\n\r";
        }
    }
    return s;
}

std::string Processor::Listline(int LineNo) {
    std::string s="";
    for (int i=0; i<Program.size(); i++) {
        if (Program[i].ProgramLine==LineNo) {
            s+=Program[i].GetString()+"\n\r";
            return s;
        }       
    }
    return s;
}


void Processor::Exit() {
    Active=false;

}