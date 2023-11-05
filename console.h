#include <string>
#include <termios.h>

#define CTRL_KEY(k) ((k) & 0x1f)
#define END_STRING 255

enum editorKey {
  BACKSPACE = 127,
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  DEL_KEY,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN,
  OPT_LEFT,
  OPT_RIGHT
};

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
