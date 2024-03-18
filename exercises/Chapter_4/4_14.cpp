#include <stdio.h>

#define swap(t, x, y) { t temp = x; x = y; y = temp; }

int main() {
  int xint = 3, yint = 7;

  swap(int, xint, yint);

  printf("xint is %d and yint is %d\n", xint, yint);

  float xfloat = 3.9, yfloat = -12.2;

  swap(float, xfloat, yfloat);

  printf("xfloat is %3.1f and yfloat is %2.1f\n", xfloat, yfloat);

  char foo[] = "foo", bar[] = "bar", *xstr = foo, *ystr = bar;

  swap(char *, xstr, ystr);

  printf("xstr is %s and ystr is %s\n", xstr, ystr);
}
