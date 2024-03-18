#include <stdio.h>
#include "getint.h"

#define SIZE 10

int main() {
  int c, m = 0, n, array[SIZE];

  for (n = 0; n < SIZE; n++) {
    while ((c = getint(&array[n])) == 0)
      ;
    if (c == EOF) {
      break;
    }
  }

  if (n > 0) {
    for (m = 0; m < n - 1; m++) {
      printf("%d, ", array[m]);
    }
    printf("%d\n", array[m]);
  }
}
