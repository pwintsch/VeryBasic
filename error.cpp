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
                    {ERR_SYNTAX, "SYNTAX ERROR: Syntax error"},
                    {ERR_NOT_EXPRESSION, "SYNTAX ERROR: Not a valid expression"},
                    {ERR_NOT_AVAILABLE, "SYNTAX ERROR: Functionality not available yet"},
                    {ERR_UNKNOWN_EXPRESSION_NODE, "SYNTAX ERROR: Unknown expression node type in evaluation"},
                    {ERR_MISMATCH_EXPRESSION_TYPES, "SYNTAX_ERROR: Mismatched data types in expression"},
                    {ERR_UNKNOWN_EXPRESSION_DATA_TYPE, " SYNTAX ERROR: Unknown data type in expression"},
                    {ERR_EXPRESSION_OPERATOR_DATATYPE, "SYNTAX ERROR: No such operator for data type in expression"},
                    {ERR_VARIABLE_NOT_FOUND, "SYNTAX ERROR: Trying to refer to non existing Variable"},
                    {ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE, "SYNTAX ERROR: Mismatched data types between variable and Expression"},
                    {ERR_BAD_PRINT_EXPRESSION, "SYNTAX ERROR: Bad expression in print statement"}
};

std::string ErrorMsg(int pID) {
    for (int i=0; i<sizeof(errors); i++) {
        if (errors[i].ID==pID) {
            return errors[i].Message;
        }
    }
    return "Unknown error";
}