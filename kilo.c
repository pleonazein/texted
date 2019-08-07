/* this project will be following the snaptoken tutorial on writing your own text editor, in C, amounting to around 1000 LOC.

   The development of this program is across 184 steps; which I shall track using git, and push to my repo "texted" */

#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define CTRL_KEY(k) ((k) & 0x1F)

struct editorConfig {
  int screenrows;
  int screencols;
  struct termios original_termios;
};

struct editorConfig E;


void editorRefreshScreen();
void disableRawMode();
void enableRawMode();
void die();
char editorReadKey();
void editorProcessKeypress();
void editorDrawRows();
void clearScreen();
int getWindowSize(int*, int*);
void initEditor();
int getCursorPosition(int*, int*);

int main() {

  enableRawMode();
  initEditor();
  editorRefreshScreen();
    
  while(1) {
    editorProcessKeypress();
  }
  
  return 0;
}

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.original_termios);
}

void die (const char *s) {
  clearScreen();
  disableRawMode();
  perror(s);
  exit(1);
}

void enableRawMode() {

  if(tcgetattr(STDIN_FILENO, &E.original_termios) == -1)
    die("tcgetattr() failure");
  
  struct termios raw = E.original_termios;
  raw.c_iflag &= ~(ICRNL|IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO|ICANON|ISIG|IEXTEN);

  // the following are miscellaneous flags that are disabled
  raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP);
  raw.c_cflag |= (CS8);

  // read() timeouts:
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tcsetattr() failure");
}

char editorReadKey() {
  int nread;
  char c;

  while((nread = read(STDIN_FILENO, &c, 1)) != 1) {
      if(nread == -1 && errno != EAGAIN)
	die("read() failure");
    }
  return c;
}

void editorProcessKeypress() {
  char c = editorReadKey();

  switch(c) {
    
  case CTRL_KEY('q'):
    clearScreen();
    disableRawMode();
    exit(0);
    break;
    
  case CTRL_KEY('l'):
    editorRefreshScreen();
    break;
    
  default:
    printf("%c\r\n",c);
    fflush(stdout);
    break;
  }
}

void editorRefreshScreen() {
  clearScreen();
  editorDrawRows();
  write(STDOUT_FILENO, "\x1b[0;0H",6);
}

void editorDrawRows() {
  for(int y=0; y < 48; y++)
    write(STDOUT_FILENO, "~\r\n",3);
}

void clearScreen() {
  write(STDOUT_FILENO, "\x1b[2J",4);
  write(STDOUT_FILENO, "\x1b[0;0H",6);
}


int getWindowSize(int *rows, int *cols) {

  struct winsize ws;

  if(1 || ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    if(write(STDOUT_FILENO, "\x1b[999C\x1b[999B",12) != 12)
      return -1;
    return getCursorPosition(rows, cols);
    
    return -1;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

void initEditor() {
  if( getWindowSize(&E.screenrows, &E.screencols) == -1)
    die("getWindowSize() failure)");
}

int getCursorPosition (int *rows, int *cols) {

  if(write(STDOUT_FILENO, "\x1b[6n",4) != 4)
    return -1;

  printf("\r\n");
  char c;

  while(read(STDIN_FILENO, &c, 1) == 1) {
    if(iscntrl(c)) {
      printf("%d\r\n",c);
    } else {
      printf("%d ('%c')\r\n",c,c);
    }
  }

  editorReadKey();

  return 0;
}


