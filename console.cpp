  
#include <iostream>
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include "console.hpp"
  
struct termios orig_termios;

Console Terminal;

// Functions in C to deal with the terminal directly which can not be wrapped in a class


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


int GetWindowSize(int *rows, int *cols) {
  struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
            return GetCursorPosition(rows, cols);
    } else  {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}


void SetCursorPosition (int row, int column) {
char terminalStr[20];
int escapeCodeLength=0;
    
    snprintf(terminalStr,20,"\x1b[%i;%iH",row,column);
    escapeCodeLength=strlen(terminalStr);
    write(STDOUT_FILENO,terminalStr,escapeCodeLength);

}

void ClearScreen() {
    write(STDOUT_FILENO, "\x1b[2J", 4);
}


void ResetScreen() {
    ClearScreen();
    SetCursorPosition(1,1); 
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


// GetStr return values:
// 0 - catastrophic failure
// x - Where x is the last key pressed before exit
// Otherwise Key pressed
// 
// Params:
// sParam Pointer to a Char Pointer, contains string to be edited or Null, should be freeable
// iExitOption - Option define which exit possibility
//  0 - Single String Mode -> Return & CTRL-C & Escape 
//  1 - Editor Mode -> Return and Up / Down / Pg-Up / PgDn CursorKeys
// iStrMaxSize - MaximumSizeOfString (0 defaults to screen width) / will be capped by room on screen

int GetStr(char **sParam, int iCursorPos, int iExitOption, int iStrMaxSize) {
int c=0;
char *sIn;
int y;
int x;
int iStartX;
int iStartY;
int iPos;
int iLen=0;
int iMaxRows, iMaxCols;


  ShowCursor();
  GetCursorPosition (&iStartY, &iStartX);
  GetWindowSize(&iMaxRows, &iMaxCols);
  int iAvailableChar=iMaxCols-iStartX;
  iStrMaxSize=(iStrMaxSize==0)?iAvailableChar:iStrMaxSize;
  iStrMaxSize=(iStrMaxSize>iAvailableChar)?iAvailableChar:iStrMaxSize;
  sIn=(char*) malloc(sizeof(char)*(iStrMaxSize+1));  
  sIn[0]='\0';
  if (*sParam!=NULL) strncpy(sIn, *sParam,iStrMaxSize);
  iLen=strlen(sIn);
  iPos=(iCursorPos<iStrMaxSize)?iCursorPos:iStrMaxSize;
  iPos=(iPos>iLen)?iLen:iPos;
  write(STDOUT_FILENO, &sIn[0], iLen);
  for (int j=iLen;j<iStrMaxSize-1;j++) write(STDOUT_FILENO, " ", sizeof(char));
  SetCursorPosition(iStartY,iStartX+iPos);
  bool bLoop=true;
  while (bLoop==true) {
    c=ReadKeyPress();
    int i=0;
    switch (c) {
      case PAGE_DOWN:
      case PAGE_UP:
      case ARROW_DOWN:
      case ARROW_UP:
      case CTRL_KEY('T'):
      case CTRL_KEY('B'):
        if (iExitOption==1) {
          bLoop=false;
        }
        break;
      case CTRL_KEY('z'):
      case '\e':
      case '\r':
        bLoop=false;
        break;
      case ARROW_LEFT:
        GetCursorPosition (&y, &x);
        if (x>iStartX) {
          SetCursorPosition (y, x-1);
          iPos--;
        }
        break;
      case OPT_LEFT:
        if (iPos>0) {
          int j=iPos-1;
          while (sIn[j]==' ' && j>0) j--; 
          for (i=j; i>0; i--) {
            if (sIn[i]==' ') { 
              i++;
              break;
            }
          }
          iPos=i;
          SetCursorPosition(iStartY,iStartX+iPos);
        }
        break;
      case OPT_RIGHT:
        for (i=iPos;i<iLen;i++) {
          if (sIn[i]==' ') { 
            if (i<iLen-1) i++;
            break;
          }
        }
        iPos=i;
        SetCursorPosition(iStartY,iStartX+iPos);
        break;
      case ARROW_RIGHT:
        GetCursorPosition (&y, &x);
        if (x<iStartX+strlen(sIn)) {
          SetCursorPosition (y, x+1);
          iPos++;
        }
        break;
      case BACKSPACE:
        if (iPos>0) {
          for (int i=iPos-1;i<=iLen;i++) {
            sIn[i]=sIn[i+1];
          }
          iPos--;
          iLen--;
          SetCursorPosition(iStartY,iStartX);
          write(STDOUT_FILENO, &sIn[0], iLen);
          SetCursorPosition(iStartY,iStartX+iLen);
          write(STDOUT_FILENO," ",1);
          SetCursorPosition(iStartY,iStartX+iPos);
        } else if (iExitOption==1) {
            bLoop=false;
        }
        break;
      case HOME_KEY:
        iPos=0;
        SetCursorPosition(iStartY,iStartX+iPos);
        break;
      case END_KEY:
        iPos=strlen(sIn);
        SetCursorPosition(iStartY,iStartX+iPos);
        break;      
      default:
        if (isascii(c) && iLen<iStrMaxSize) {
          // adapt to insert a character at any place in string by shifting entire string from ipos by 1
          iLen++;
          for (int i=iLen;i>=iPos;i--) {
            sIn[i]=sIn[i-1];
          }
          sIn[iPos++]=c;
          SetCursorPosition(iStartY,iStartX);
          write(STDOUT_FILENO, &sIn[0], iLen);
          write(STDOUT_FILENO," ",1);
          SetCursorPosition(iStartY,iStartX+iPos);
        }
        break;
    }
  }
//  write(STDOUT_FILENO,"\n\r",2);
  
  if (c=='\r' || iExitOption==1) {
    char *sReturn;
    sReturn=(char*) malloc((sizeof(char))*(strlen(sIn)+1));
    strcpy(sReturn,sIn);
    if (*sParam!=NULL) free(*sParam);
    *sParam=sReturn;
  }
  if (sIn!=NULL) free(sIn);
//  SetCursor(iStartY,iStartX);
  HideCursor();
  return c;
}


void Console::Initialise() {
    GetWindowSize(&height, &width);
    for (int i=0;i<CONSOLE_LENGTH;i++) gsConsoleHistory[i]=NULL;
    giConsoleHistoryLen=0;
    WriteFStringLn ("Teminal Initialised, %d columns by %d rows", width, height);
    strcpy(gsConsoleStr,"");
    EnableRawMode();
    HideCursor();
}


void Console::print(const std::string& str) {
  std::cout << str << std::endl;
}

void Console::Clear() {
  ResetScreen();
}

int Console::get_height() {
  return height;
}

int Console::get_width() {
  return width;
}

void Console::Beep() {
  char sBeep[2]="\a";
  Print(sBeep);
}

void Console::Print(char *S) {
  write(STDOUT_FILENO,S,strlen(S));
}

void Console::PrintLn(char *S) {
  write(STDOUT_FILENO,S,strlen(S));
  write(STDOUT_FILENO,"\n\r",2);
}

void Console::ReversePrint(char *S) {
  write(STDOUT_FILENO,"\x1b[7m",4);
  write(STDOUT_FILENO,S,strlen(S));
  write(STDOUT_FILENO,"\x1b[m",3);
}

void Console::SetInkRed() {
  write(STDOUT_FILENO,"\x1b[31m", 5);
}

void Console::SetInkGreen() {
  write(STDOUT_FILENO,"\x1b[32m", 5);
}

void Console::SetInkStd() {
  write(STDOUT_FILENO,"\x1b…[39m", 5);
}


void Console::WriteFString(const char* format, ... ) {
	char sStr[255];
  va_list args;
  va_start( args, format );
  vsnprintf( sStr, 255, format, args );
  va_end( args );
	Print(sStr);
}

void Console::WriteFStringLn(const char* format, ... ) {
	char sStr[255];
    va_list args;
    va_start( args, format );
    vsnprintf( sStr, 255, format, args );
    va_end( args );
	PrintLn (sStr);
}

void Console::Write(const char *sTxt) {
  char sStr[255];
  strncpy(sStr,sTxt,255);  
	Print(sStr);
}


void Console::WriteLn(const char *sTxt) {
  char sStr[255];
  int iLen=strlen(sTxt);
  if (iLen<255) {
    strncpy(sStr,sTxt,255);  
    PrintLn(sStr);
  } else {
    // split string into multiple lines of 255 chars
    int iPos=0;
    while (iPos<iLen) {
      int LentoCopy=(iLen-iPos<255)?iLen-iPos:254;
      strncpy(sStr,&sTxt[iPos],LentoCopy);
      sStr[LentoCopy]='\0'; // ensure null termination
      Print(sStr);
      iPos+=254;
    }
    strncpy(sStr,"",1);
    PrintLn(sStr);
  }
}

int Console::GetString (std::string &sInput) {
char *sIn=NULL;
int i=GetStr(&sIn,0,0,0);
  sInput=sIn;
  if (sIn!=NULL) free (sIn);
  return i;
}


void Console::GetConsoleInput (std::string &sInput, int iNoOfChar) {

char *sIn=NULL;
int iY;
int iX;
char sPromptStr[5]="> ";
char sEmptyStr[5]="";

	WriteLn (sEmptyStr);
	Write(sPromptStr);
	GetCursorPosition (&iY, &iX);
	int i=0;
	int iHistoryPos=giConsoleHistoryLen;
	if (gsConsoleStr[0] != '\0') {
		if (sIn!=NULL) free (sIn);
		sIn=strdup (gsConsoleStr);
		strcpy (gsConsoleStr,"");
	}
	do {
		SetCursorPosition (iY, iX);
		i=GetStr(&sIn,END_STRING,1,0);
		if (i==ARROW_UP && iHistoryPos>0) {
			if (sIn!=NULL)  free(sIn);
			sIn= strdup(gsConsoleHistory[iHistoryPos-1]);
			if (iHistoryPos>0) iHistoryPos--;
		}
		if (i==ARROW_DOWN && iHistoryPos<giConsoleHistoryLen) {
			if (sIn!=NULL)  free(sIn);
			sIn=strdup(gsConsoleHistory[iHistoryPos]);
			if (iHistoryPos<giConsoleHistoryLen) iHistoryPos++;
		}
	} while (!(i=='\r' || i==CTRL_KEY('c')));

	if (giConsoleHistoryLen<CONSOLE_LENGTH) {
		gsConsoleHistory[giConsoleHistoryLen]=strdup(sIn);
		giConsoleHistoryLen++;
	} else {
		for (int j=1;j<giConsoleHistoryLen;j++) gsConsoleHistory[j-1]=gsConsoleHistory[j];
		if (gsConsoleHistory[giConsoleHistoryLen-1]!=NULL) free(gsConsoleHistory[giConsoleHistoryLen-1]);
		gsConsoleHistory[giConsoleHistoryLen-1]=strdup(sIn);
	}
	WriteLn(sEmptyStr);
	// strcpy(sParam, sIn);
  sInput=sIn;
	if (sIn!=NULL) free (sIn);
}


bool Console::GetYNConfirmation(const char *sQuestion) {
char sEmptyStr[5]="";
	WriteFString ("%s ? (Y/n)", sQuestion);
	char *tmpS=NULL;
	GetStr(&tmpS,0,0,1);
	if ((tmpS[0]=='Y') || (tmpS[0]=='y')) {
		free(tmpS);
		WriteLn(sEmptyStr);
		return true;
	}
	free(tmpS);
	WriteLn(sEmptyStr);
	return false;
}

void Console::MoveCursorToNextTab() {

int iY;
int iX;
int iMaxRows, iMaxCols;
const int TABSIZE=8;

  GetCursorPosition (&iY, &iX);
  GetWindowSize(&iMaxRows, &iMaxCols);
  int iAvailableChar=iMaxCols-iX;
  if (iAvailableChar<TABSIZE) {
    WriteLn("");
  } else {
    int iSpaces=TABSIZE-(iX%TABSIZE); 
    for (int i=0;i<iSpaces;i++) Write(" ");
  }
}


void Console::MoveCursorToColumn(int iColumn) {

int iY;
int iX;
int iMaxRows, iMaxCols;


  GetCursorPosition (&iY, &iX);
  GetWindowSize(&iMaxRows, &iMaxCols);
  iColumn=iColumn%iMaxCols;
  if (iColumn>iX) {
    int iSpaces=iColumn-iX;
    for (int i=0;i<iSpaces;i++) Write(" ");
  } else {
    WriteLn("");
    for (int i=0;i<iColumn;i++) Write(" ");
  }
}

void Console::GetCursorPos(int &iY, int &iX) {
  GetCursorPosition (&iY, &iX);
}


void Console::MoveCursor(int iY, int iX) {
  SetCursorPosition(iY,iX);
}


Console::~Console() {
	ShowCursor();
	DisableRawMode();
}