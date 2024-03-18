#include <stdio.h>

bool streq(char *s, char *t) {
  int n;
  for (n = 0; *(s + n) != '\0'; n++) {
    if (*(s + n) != *(t + n)) {
      return false;
    }
  }
  return *(t + n) == '\0';
}

bool strend(char *s, char *t) {
  if (*t == '\0') {
    return true;
  }
  int n;
  for (n = 0; *(s + n) != '\0'; n++) {
    if (streq(s + n, t)) {
      return true;
    }
  }
  return false;
}

void teststrend(char *s, char *t) {
  printf("\"%s\" %s end in \"%s\"\n", s, strend(s, t) ? "does" : "doesn't", t);
}

int main() {
  char foo[] = "foo", bar[] = "bar", foobar[] = "foobar",
    foobarbaz[] = "foobarbaz", empty[] = "", *first, *second;

  first = foobar;
  second = bar;
  teststrend(first, second);

  first = foo;
  second = bar;
  teststrend(first, second);

  first = foobarbaz;
  second = bar;
  teststrend(first, second);

  first = bar;
  second = bar;
  teststrend(first, second);

  first = bar;
  second = empty;
  teststrend(first, second);
}
