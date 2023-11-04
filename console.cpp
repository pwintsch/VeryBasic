  
#include <iostream>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "console.h"
  

  Console::Console() {
    get_terminal_size();
  }
  
  void Console::print(const std::string& str) {
    std::cout << str << std::endl;
  }

  void Console::clear() {
    write(STDOUT_FILENO, "\x1b[2J", 4);
  }

  void Console::get_terminal_size() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    width = w.ws_col;
    height = w.ws_row;
  }


  int Console::get_height() {
    return height;
  }

  int Console::get_width() {
    return width;
  }

  void Console::set_cursor_position(int x, int y) {
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", y, x);
    write(STDOUT_FILENO, buf, strlen(buf));
  }