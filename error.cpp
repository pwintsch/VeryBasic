#include "error.hpp"

struct Error {
    int ID;
    std::string Message;
};

Error errors[] = {  {NO_ERROR, "OK"},
                    {ERR_TOKEN_OPEN_STRING, "SYNTAX ERROR: Unclosed quote mark in line"},
                    {ERR_BAD_BRACKET, "SYNTAX ERROR: Bracket error - check open/close"},
                    {ERR_BAD_COMMAND, "SYNTAX ERROR: Unknown command"},
                    {ERR_DIRECTCOMMAND_IN_PROGRAM, "SYNTAX ERROR: Direct command not allowed in program"},
                    {ERR_NO_MATCHING_SYNTAX, "SYNTAX ERROR: No matching syntax found for command"},
                    {ERR_BAD_EXPRESSION, "SYNTAX ERROR: Expression contains invalid tokens"},
                    {ERR_BAD_FUNCTION_BRACKET, "SYNTAX ERROR: Function call with invalid brackets"},
                    {ERR_NO_LINE_NUMBER, "SYNTAX ERROR: No such line number"},
                    {ERR_LINE_ALREADY_EXISTS, "SYNTAX ERROR: Line already exists"},
};

std::string ErrorMsg(int pID) {
    for (int i=0; i<sizeof(errors); i++) {
        if (errors[i].ID==pID) {
            return errors[i].Message;
        }
    }
    return "Unknown error";
}