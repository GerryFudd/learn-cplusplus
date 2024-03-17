#include <stdio.h>

int main() {
  int c;
  int test = (c = getchar()) != EOF;
    printf("%d", test);
  while (test) {
    putchar(c);
    test = (c = getchar()) != EOF;
    printf("%d", test);
  }
}
