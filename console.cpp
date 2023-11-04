  
#include <iostream>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "console.h"
  
  void Console::print(const std::string& str) {
    std::cout << str << std::endl;
  }

  void Console::clear() {
    write(STDOUT_FILENO, "\x1b[2J", 4);
  }