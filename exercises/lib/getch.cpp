#include <stdio.h>
#define BUFFSIZE 500

char buf[BUFFSIZE];
int bufp = 0;

int getch(void) {
  return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
  if (bufp >= BUFFSIZE) {
    printf("ungetch: too many characters in buffer.\n");
  } else {
    buf[bufp++] = c;
  }
}
