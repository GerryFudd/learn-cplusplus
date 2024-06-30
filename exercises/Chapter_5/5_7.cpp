#include <stdio.h>
#include <chrono> 
#include <string.h>
#include "getline.h"

#define MAXLEN 1000
#define ALLOCSIZE 500000
#define MAXLINES 1000

char *lineptr[MAXLINES];

int readlines(char *lineptr[], char linearr[][MAXLEN], int maxlines) {
  int len, nlines;
  char *p;

  nlines = 0;
  while (nlines < maxlines && (len = getline(linearr[nlines], MAXLEN)) > 0) {
    linearr[nlines][len - 1] = '\0';
    p = linearr[nlines];
    lineptr[nlines++] = p;
  }
  if (nlines >= maxlines) {
    return -1;
  }
  return nlines;
}

int main() {
  char linearr[MAXLINES][MAXLEN];
  printf("About to use new readlines.\n");
  int nlines;
  auto start{std::chrono::steady_clock::now()};
  if ((nlines = readlines(lineptr, linearr, MAXLINES)) >= 0) {
    auto end{std::chrono::steady_clock::now()};
    printf("This took %fms\n", std::chrono::duration<double>{end - start} * 1000);
  } else {
    printf("error: input too big to sort\n");
    return 1;
  }
  return 0;
}
