/* this project will be following the snaptoken tutorial on writing your own text editor, in C, amounting to around 1000 LOC.

   The development of this program is across 184 steps; which I shall track using git, and push to my repo "texted" */

#include <unistd.h>
#include <termios.h>

struct termios original_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

void enableRawMode() {

  tcgetattr(STDIN_FILENO, &original_termios);
  struct termios raw = original_termios;
  raw.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  char c;

  enableRawMode();
  
  while (read(STDIN_FILENO, &c, 1) == 1 && c!= 'q');
  disableRawMode();
  return 0;
}
