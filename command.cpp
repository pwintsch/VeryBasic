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