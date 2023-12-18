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


std::string ErrorMsg(int pID);


 #endif /* ERROR_H_ */