#include <iostream>

void strncpy(char *, char *, int);
void strncat(char *, char *, int);

void strncpy(char *target, char *source, int n) {
  int cursor = 0;
  while (cursor++ < n && (*target++ = *source++) != '\0') {}
  *target = '\0';
}

void strncat(char *target, char *source, int n) {
  int cursor = 0;
  while (*target++ != '\0') {}
  target--;
  while (cursor++ < n && (*target++ = *source++) != '\0') {}
  *target = '\0';
}


int main() {
  char *s = new char[101], source[] = "This is the text to copy from.", *t;
  t = source;
  strncpy(s, t, 0);
  std::cout << s << std::endl;
  strncpy(s, t, 3);
  std::cout << s << std::endl;
  strncat(s, t+3, 3);
  std::cout << s << std::endl;
  strncat(s, t+6, 10);
  std::cout << s << std::endl;
  strncpy(s, t, 100);
  std::cout << s << std::endl;
  return 0;
}