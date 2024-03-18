#include <stdio.h>
#define MAXPRINTBUFF 100

void reverseran(char *start, char *end) {
  if (start >= end) {
    // If end is not after start, do nothing.
    return;
  }
  char temp = *start;
  *start = *end;
  *end = temp;

  reverseran(start+1,end-1);
}

void reverse(char s[]) {
  int length = 0;
  while (s[length] != '\0') {
    length++;
  }
  reverseran(s, s + length - 1);
}

int main() {
  char text[] = "Food is good!";
  reverse(text);
  printf("%s\n", text);
  char empty_text[] = "";
  reverse(empty_text);
  printf("%s\n", empty_text);
  char singleton[] = "a";
  reverse(singleton);
  printf("%s\n", singleton);
}

