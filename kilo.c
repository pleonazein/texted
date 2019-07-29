/* this project will be following the snaptoken tutorial on writing your own text editor, in C, amounting to around 1000 LOC.

   The development of this program is across 184 steps; which I shall track using git, and push to my repo "texted" */

#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>

struct termios original_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

void enableRawMode() {

  tcgetattr(STDIN_FILENO, &original_termios);
  struct termios raw = original_termios;
  raw.c_iflag &= ~(ICRNL|IXON);
  raw.c_lflag &= ~(ECHO|ICANON|ISIG);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  char c;

  enableRawMode();
  
  while (read(STDIN_FILENO, &c, 1) == 1 && c!= 'q') {
    if(iscntrl(c)) {
      printf("%d\n", c);
    } else {
      printf("%d ('%c')\n", c, c);
    }
  }
  disableRawMode();
  return 0;
}
