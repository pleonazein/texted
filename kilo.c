/* this project will be following the snaptoken tutorial on writing your own text editor, in C, amounting to around 1000 LOC.

   The development of this program is across 184 steps; which I shall track using git, and push to my repo "texted" */

#include <unistd.h>


int main() {
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1);
  return 0;
}
