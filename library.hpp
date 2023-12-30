#ifndef LIBRARY_H_
#define LIBRARY_H_
#include "command.hpp"



int LetCmd(Command MyCommand);

extern int (*DirectCommandPtr[])(Command MyCommand);
extern int (*CommandPtr[])(Command MyCommand);


#endif /* LIBRARY_H_ */