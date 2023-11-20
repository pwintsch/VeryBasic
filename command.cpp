#include "command.hpp"
#include "error.hpp"
#include "token.hpp"
#include "syntax.hpp"

Command::~Command() {
    // TODO Auto-generated destructor stub
    Parameters.clear();
}

int Command::Initialise(std::vector<Token> &pTokens) {

    Parameters=pTokens;
    Type=Parameters[0].Type;
    ID=Parameters[0].ID;
    return NO_ERROR;
}

std::string Command::GetString() {
    std::string s="";
    s="Command: " + std::to_string(ID) + " - " + std::to_string(Type) + " :  ";
    for (int i=0; i<Parameters.size(); i++) {
        if (i>0) {
            s=s + ", ";
        }
        s=s + " - {" + std::to_string(Parameters[i].Type) + ":" + std::to_string(Parameters[i].ID) + ":" + Parameters[i].Value + "} ";
    }
    s=s+"\n\r";
    return s;
}