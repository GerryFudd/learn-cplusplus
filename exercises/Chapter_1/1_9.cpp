#include <stdio.h>
#include <ctype.h>

int main() {
  int c;
  bool blank_section = false;
  while ((c = getchar()) != EOF) {
    if (isblank(c)) {
      blank_section = true;
    } else {
      if (blank_section) {
        blank_section = false;
        putchar(' ');
      }
      putchar(c);
    }
  }
  if (blank_section) {
    blank_section = false;
    putchar(' ');
  }
}
