#include <stdio.h>

int main() {
  printf("Celsius\tFahrenheit\n");
  float fahr, celsius;
  int lower, upper, step;

  lower = -40;
  upper = 160;
  step = 20;

  celsius = lower;
  while (celsius <= upper) {
    fahr = celsius * 9.0/5.0 + 32.0;
    printf("%3.0f\t\t%6.1f\n", celsius, fahr);
    celsius = celsius + step;
  }
}
