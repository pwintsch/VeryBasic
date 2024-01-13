#ifndef LIBRARY_H_
#define LIBRARY_H_
#include "command.hpp"



int LetCmd(Command MyCommand);

extern int (*DirectCommandPtr[])(Command MyCommand);
extern int (*CommandPtr[])(Command MyCommand);
extern int (*FunctionPtr[])(CommandNode &Node, int &ReturnType, float &NumResult, std::string &StrResult);


#endif /* LIBRARY_H_ */