#include <string>
#include <termios.h>

class Console {
  int width;
  int height;
  struct termios orig_termios;
  void get_terminal_size();

public:
  Console();
  void print(const std::string& str);
  void clear();
  int get_width();
  int get_height();
  void set_cursor_position(int x, int y);

  ~Console();
};
