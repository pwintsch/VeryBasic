#ifndef LIBRARY_H_
#define LIBRARY_H_
#include "command.hpp"





extern int (*DirectCommandPtr[])(Command MyCommand);
extern int (*CommandPtr[])(Command MyCommand);


#endif /* LIBRARY_H_ */