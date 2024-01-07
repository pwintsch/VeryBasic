#ifndef ERROR_H_
#define ERROR_H_

#include <string>


#define NO_ERROR 0
#define ERR_TOKEN_OPEN_STRING 1
#define ERR_BAD_BRACKET 2
#define ERR_BAD_COMMAND 3
#define ERR_DIRECTCOMMAND_IN_PROGRAM 4
#define ERR_NO_MATCHING_SYNTAX 5
#define ERR_BAD_EXPRESSION 6
#define ERR_BAD_FUNCTION_BRACKET 7
#define ERR_NO_LINE_NUMBER 8
#define ERR_LINE_ALREADY_EXISTS 9
#define ERR_SYNTAX 10
#define ERR_NOT_EXPRESSION 11
#define ERR_NOT_AVAILABLE 12
#define ERR_UNKNOWN_EXPRESSION_NODE 13
#define ERR_MISMATCH_EXPRESSION_TYPES 14
#define ERR_UNKNOWN_EXPRESSION_DATA_TYPE 15
#define ERR_EXPRESSION_OPERATOR_DATATYPE 16
#define ERR_VARIABLE_NOT_FOUND 17
#define ERR_MISMATCH_EXPRESSION_TO_VARIABLE_TYPE 18
#define ERR_BAD_PRINT_EXPRESSION 19
#define ERR_BAD_PRINT_COORDINATES 20
#define ERR_BAD_INPUT_EXPRESSION 21
#define ERR_BAD_INPUT_COORDINATES 22
#define ERR_CMD_INPUT_ERROR 23
#define ERR_CMD_INPUT_BREAK 24
#define ERR_CMD_IF_EXPRESSION_NOT_VALUE 25
#define ERR_CALLSTACK_EMPTY 26
#define ERR_BAD_DIM_COMMAND 27
#define ERR_DIM_ARRAY_ALREADY_EXISTS 28
#define ERR_ARRAY_ITEM_NOT_FOUND 29 
#define ERR_LAZY_PROGRAMMER 30
#define ERR_DIM_ARRAY_DONT_MATCH 31
#define ERR_CMD_OPTION_BASE_OUT_OF_RANGE 32
#define ERR_FILE_NOT_FOUND 33
#define ERR_FORSTACK_EMPTY 34
#define ERR_FORSTACK_MISMATCH 35



std::string ErrorMsg(int pID);


 #endif /* ERROR_H_ */