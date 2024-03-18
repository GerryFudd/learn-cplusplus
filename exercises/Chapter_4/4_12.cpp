#include <stdio.h>
#define MAXPRINTBUFF 100

char *nextchar(int n, char *s) {
  if (n < 0) {
    *s = '-';
    return nextchar(-n, s + 1);
  }
  char *result = n / 10 ? nextchar(n / 10, s) : s;
  *(result++) = n % 10 + '0';
  return result;
}

void itoa(int n, char s[]) {
  *nextchar(n, s) = '\0';
}

int main() {
  char printbuf[MAXPRINTBUFF];
  itoa(19026, printbuf);
  printf("%s\n", printbuf);
  itoa(-824, printbuf);
  printf("%s\n", printbuf);
  itoa(0xff, printbuf);
  printf("%s\n", printbuf);
}

