  
#include <iostream>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "console.h"
  
struct termios orig_termios;


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

int GetCursor (int *rows, int *cols) {
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