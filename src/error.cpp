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
                    {ERR_BAD_PRINT_EXPRESSION, "SYNTAX ERROR: Bad expression in print statement"},
                    {ERR_BAD_PRINT_COORDINATES, "SYNTAX ERROR: Trying to print outside Terminal window dimensions"},
                    {ERR_BAD_INPUT_EXPRESSION, "SYNTAX ERROR: Bad expression in input statement"},
                    {ERR_BAD_INPUT_COORDINATES, "SYNTAX ERROR: Trying to input outside Terminal window dimensions"},
                    {ERR_CMD_INPUT_ERROR, "SYNTAX ERROR: Error in INPUT command"},
                    {ERR_CMD_INPUT_BREAK, "SYNTAX ERROR: Break requested in INPUT"},
                    {ERR_CMD_IF_EXPRESSION_NOT_VALUE, "SYNTAX ERROR: Invalid IF condition, it does not evaluate to a value"},
                    {ERR_CALLSTACK_EMPTY, "SYNTAX ERROR: RETURN without GOSUB or FUNCTION CALL"},
                    {ERR_BAD_DIM_COMMAND, "SYNTAX ERROR: Bad DIM command parametres, must be an undeclared variable with dimension parametres"},
                    {ERR_DIM_ARRAY_ALREADY_EXISTS, "SYNTAX ERROR: DIM nor possible, Array already exists"},
                    {ERR_ARRAY_ITEM_NOT_FOUND, "SYNTAX ERROR: Array item not found, possibly not declared"},
                    {ERR_LAZY_PROGRAMMER, "SYNTAX ERROR: Lazy programmer, not implemented yet"},
                    {ERR_DIM_ARRAY_DONT_MATCH, "SYNTAX ERROR: Array dimensions don't match"},
                    {ERR_CMD_OPTION_BASE_OUT_OF_RANGE, "SYNTAX ERROR: Option Base out of range, must be 0 or 1"},
                    {ERR_FILE_NOT_FOUND, "SYNTAX ERROR: File not found"},
                    {ERR_FORSTACK_EMPTY, "SYNTAX ERROR: NEXT without FOR"},
                    {ERR_FORSTACK_MISMATCH, "SYNTAX ERROR: Mismatched FOR/NEXT loop"},
                    {ERR_BAD_FOR_COMMAND, "SYNTAX ERROR: Bad FOR command syntax"},
                    {ERR_NO_READ_DATA, "EXECUTION ERROR: No DATA items to read"},
                    {ERR_BAD_RESTORE_LINE, "SYNTAX ERROR: No such line number for RESTORE"},
                    {ERR_BAD_FUNCTION_PARAM_NO, "SYNTAX ERROR: Bad number of parameters for function call"},
                    {ERR_BAD_FUNCTION_PARAM_TYPE, "SYNTAX ERROR: Bad parameter type for function call"},
                    {ERR_FUNCTION_ALREADY_EXISTS, "SYNTAX ERROR: Illegal function FN name - already exists"},
                    {ERR_FUNCTION_NOT_FOUND, "SYNTAX ERROR: User Function FN not found"},
                    {ERR_DEFN_PARAM_NOT_VARIABLE, "SYNTAX ERROR: DEF FN parameter must be a variable"},
                    {ERR_REPEATSTACK_EMPTY, "SYNTAX ERROR: UNTIL without REPEAT"},
                    {ERR_WEND_NOT_FOUND, "SYNTAX ERROR: No Matching WEND for WHILE statement"},
                    {ERR_WHILESTACK_EMPTY, "SYNTAX ERROR: WEND without WHILE"},
                    {ERR_FUNC_PARAM_EMPTYSTRING, "SYNTAX ERROR: Empty string in function parameter"}
};

std::string ErrorMsg(int pID) {
    for (int i=0; i<sizeof(errors); i++) {
        if (errors[i].ID==pID) {
            std::string s="Error #" + std::to_string(i) + ", " + errors[i].Message;
            return s;
        }
    }
    return "Unknown error";
}