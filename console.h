#include <string>

class Console {
  int width;
  int height;
  void get_terminal_size();
public:
  Console();
  void print(const std::string& str);
  void clear();
  int get_width();
  int get_height();
  void set_cursor_position(int x, int y);
};
