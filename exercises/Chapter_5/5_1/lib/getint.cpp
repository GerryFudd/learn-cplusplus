#include <stdio.h>
#include <ctype.h>
#include "getch.h"

int getint(int *pn) {
  int c, sign;

  while (isspace(c = getch()))
    ;
  sign = (c == '-') ? -1 : 1;
  if (c == '+' || c == '-') {
    c = getch();
  }
  if (!isdigit(c) && c != EOF) {
    return 0;
  } 
  for (*pn = 0; isdigit(c); c = getch()) {
    *pn = 10 * *pn + (c - '0');
  }
  *pn *= sign;
  if (c != EOF) {
    ungetch(c);
  }
  return c;
}