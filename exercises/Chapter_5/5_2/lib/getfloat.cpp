#include <ctype.h>
#include <stdio.h>
#include "getch.h"

int getfloat(float *pn) {
  int c, denom;
  float sign;

  while (isspace(c = getch()))
    ;
  sign = (c == '-') ? -1 : 1;
  if (c == '+' || c == '-') {
    c = getch();
  }
  if (!isdigit(c) && c != '.' && c != EOF) {
    return 0;
  } 
  for (*pn = 0; isdigit(c); c = getch()) {
    *pn = 10 * *pn + (c - '0');
  }
  if (c == '.') {
    c = getch();
    for (denom = 1; isdigit(c); c = getch(), denom *= 10) {
      *pn = 10 * *pn + (c - '0');
    }
    *pn /= denom;
  }
  *pn *= sign;
  if (c != EOF) {
    ungetch(c);
  }
  return c;
}
