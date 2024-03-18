#include <stdio.h>

int main() {
  int test;
  while ((test = (getchar() != EOF))) {
    printf("%d", test);
  }
  printf("%d\n", test);
}
