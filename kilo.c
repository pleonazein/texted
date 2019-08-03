/* this project will be following the snaptoken tutorial on writing your own text editor, in C, amounting to around 1000 LOC.

   The development of this program is across 184 steps; which I shall track using git, and push to my repo "texted" */

#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>

#define CTRL_KEY(k) ((k) & 0x1F)

struct termios original_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

void die (const char *s) {
  perror(s);
  exit(1);
}

void enableRawMode() {

  if(tcgetattr(STDIN_FILENO, &original_termios) == -1)
    die("tcgetattr() failure");
  
  struct termios raw = original_termios;
  raw.c_iflag &= ~(ICRNL|IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO|ICANON|ISIG|IEXTEN);

  // the following are miscellaneous flags that are disabled
  raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP);
  raw.c_cflag |= (CS8);

  // read timeouts:
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tcsetattr() failure");
}

int main() {

  enableRawMode();

  char c;

  while(1) {
    c='\0';

    if(read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
      die("read() failure");
    if( iscntrl(c) )
      printf("%d\r\n",c);
    else
      printf("%d ('%c')\r\n",c,c);
    if(c == CTRL_KEY('q'))
      break;
  }
  
  disableRawMode();
  return 0;
}
