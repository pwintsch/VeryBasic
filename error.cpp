#include "error.hpp"

struct Error {
    int ID;
    std::string Message;
};

Error errors[] = {  {NO_ERROR, "OK"},
                    {ERR_TOKEN_OPEN_STRING, "SYNTAX ERROR: unclosed quote mark in line"},
                    {ERR_BAD_BRACKET, "SYNTAX ERROR: unclosedbracket"}
};

std::string ErrorMsg(int pID) {
    for (int i=0; i<sizeof(errors); i++) {
        if (errors[i].ID==pID) {
            return errors[i].Message;
        }
    }
    return "Unknown error";
}