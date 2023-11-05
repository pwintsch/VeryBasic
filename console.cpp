  
#include <iostream>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "console.h"
  
struct termios orig_termios;


// Functions in C to deal with the terminal directly which can not be wrapped in a class

void SetCursor (int row, int column) {
char terminalStr[20];
int escapeCodeLength=0;
    
    snprintf(terminalStr,20,"\x1b[%i;%iH",row,column);
    escapeCodeLength=strlen(terminalStr);
    write(STDOUT_FILENO,terminalStr,escapeCodeLength);

}


void ResetScreen() {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    SetCursor(1,1); 
//    write(STDOUT_FILENO, "\x1b[H", 3);
}

void HideCursor() {
    write(STDOUT_FILENO, "\x1b[?25l", 6);
}

void ShowCursor() {
    write(STDOUT_FILENO, "\x1b[?25h", 6);
}


void die(const char *s) {
    ResetScreen();
    perror(s);
    exit(1);
}

void DisableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}

void EnableRawMode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  atexit(DisableRawMode);
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}


int GetCursorPosition (int *rows, int *cols) {
  char buf[32];
  unsigned int i = 0;
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;
  while (i < sizeof(buf) - 1) {
    if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
    if (buf[i] == 'R') break;
    i++;
  }
  buf[i] = '\0';
  if (buf[0] != '\x1b' || buf[1] != '[') return -1;
  if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;
  return 0;
} 




int ReadKeyPress() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }


  if (c == '\x1b') {
    char seq[3];
    if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
    if (read(STDIN_FILENO, &seq[1], 1) != 1) {
      switch (seq[0]) {
            case 'b': return OPT_LEFT;
            case 'f': return OPT_RIGHT;
            default: return '\x1b';
      }
    }
    if (seq[0] == '[') {
      if (seq[1] >= '0' && seq[1] <= '9') {
        if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
        if (seq[2] == '~') {
          switch (seq[1]) {
            case '1': return HOME_KEY;
            case '3': return DEL_KEY;
            case '4': return END_KEY;
            case '5': return PAGE_UP;
            case '6': return PAGE_DOWN;
            case '7': return HOME_KEY;
            case '8': return END_KEY;
          }
        }
      } else {
        switch (seq[1]) {
          case 'A': return ARROW_UP;
          case 'B': return ARROW_DOWN;
          case 'C': return ARROW_RIGHT;
          case 'D': return ARROW_LEFT;
          case 'H': return HOME_KEY;
          case 'F': return END_KEY;
        }
      }
    } else if (seq[0] == 'O') {
      switch (seq[1]) {
        case 'H': return HOME_KEY;
        case 'F': return END_KEY;
      }
    }
    return '\x1b';
  } else {
    return c;
  }
}


char CheckKeyPress() {
  int nread;
  char c;
  nread = read(STDIN_FILENO, &c, 1);
  if (nread == -1 && errno != EAGAIN) die("read");
  if (nread==0) c='\0';
  return c;
}





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

  Console::~Console() {
    
  }