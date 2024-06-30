#include <stdio.h>
#include <chrono> 
#include <string.h>
#include "getline.h"

#define MAXLEN 1000
#define ALLOCSIZE 500000
#define MAXLINES 1000

char *alloc(int);
void strcpy(char *, char *);

char *lineptr[MAXLINES];


int readlines_old(char *lineptr[], int maxlines) {
  int len, nlines;
  char *p, line[MAXLEN];

  nlines = 0;
  while ((len = getline(line, MAXLEN)) > 0) {
    if (nlines >= maxlines || (p = alloc(len)) == NULL) {
      return -1;
    }
    line[len - 1] = '\0';
    strcpy(p, line);
    lineptr[nlines++] = p;
  }
  return nlines;
}

int main() {
  printf("About to use old readlines.\n");
  auto start{std::chrono::steady_clock::now()};
  int nlines;
  if ((nlines = readlines_old(lineptr, MAXLINES)) >= 0) {
    auto end{std::chrono::steady_clock::now()};
    printf("This took %fms\n", std::chrono::duration<double>{end - start} * 1000);
  } else {
    printf("error: input too big to sort\n");
    return 1;
  }
  return 0;
}

static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf; // next free position
char *alloc(int n) {
  if (allocbuf + ALLOCSIZE - allocp >= n) {
    allocp += n;
    return allocp - n;
  }
  return 0;
}

void strcpy(char *target, char *source) {
  while ((*target++ = *source++) != '\0') {}
}
