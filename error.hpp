#ifndef ERROR_H_
#define ERROR_H_

#include <string>


#define NO_ERROR 0
#define ERR_TOKEN_OPEN_STRING 1
#define ERR_BAD_BRACKET 2
#define ERR_BAD_COMMAND 3
#define ERR_DIRECTCOMMAND_IN_PROGRAM 4
#define ERR_NO_MATCHING_SYNTAX 5


std::string ErrorMsg(int pID);


 #endif /* ERROR_H_ */