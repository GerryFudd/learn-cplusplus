#include <stdio.h>
#include <ctype.h>

int main() {
  int c, blanks = 0;
  while ((c = getchar()) != EOF) {
    if (isblank(c)) {
      blanks++;
    }
  }
  printf("There are %d blanks.\n", blanks);
}
