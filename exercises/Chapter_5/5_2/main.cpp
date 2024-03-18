#include <stdio.h>
#include "getfloat.h"

#define SIZE 20

int main() {
  float numbers[SIZE];
  int c, m, n;

  for (m = 0; m < SIZE; m++) {
    while ((c = getfloat(&numbers[m])) == 0)
      ;
    if (c == EOF) {
      break;
    }
  }
  if (m > 0) {
    for (n = 0; n < m - 1; n++) {
      printf("%f, ", numbers[n]);
    }
    printf("%f\n", numbers[m - 1]);
  }
}
