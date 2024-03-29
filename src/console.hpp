#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <string>
#include <termios.h>

#define CTRL_KEY(k) ((k) & 0x1f)
#define END_STRING 255
#define CONSOLE_LENGTH 20

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
  char *gsConsoleHistory[CONSOLE_LENGTH];
  int giConsoleHistoryLen;
  char gsConsoleStr[255];

public:
  void Initialise();
  void print(const std::string& str);
  void Clear();
  int get_width();
  int get_height();
  void Beep();
  void Print(char *S);
  void PrintLn(char *S);
  void ReversePrint(char *S);
  void SetInkRed();
  void SetInkGreen();
  void SetInkStd(); 
  void WriteFString(const char* format, ... );
  void WriteFStringLn(const char* format, ... );
  void Write(const char *sTxt);
  void WriteLn(const char *sTxt);
  void GetConsoleInput (std::string &sInput, int iNoOfChar);
  char CheckKeyPress();
  int GetString (std::string &sInput);
  bool GetYNConfirmation(const char *sQuestion);
  void MoveCursorToNextTab();
  void MoveCursorToColumn(int iColumn);
  void GetCursorPos(int &iY, int &iX);
  void MoveCursor(int iY, int iX);
  void SetNextConsoleInput(std::string sInput);
  ~Console();
};

extern Console Terminal;

#endif /* CONSOLE_H_ */