#include <stdio.h>
#include <ctype.h>

#define MAXLEN 10

int main() {
  int c, current_length = 0, longest_word = 0, highest_count = 0;
  int histogram[MAXLEN];
  for (int i = 0; i < MAXLEN; i++) {
    histogram[i] = 0;
  }

  while ((c = getchar()) != EOF) {
    if (isblank(c) || c == '\n') {
      if (current_length > 0) {
        if (current_length > longest_word) {
          longest_word = current_length;
        }
        histogram[current_length-1]++;
        if (histogram[current_length-1] > highest_count) {
          highest_count = histogram[current_length-1];
        }
      }
      current_length = 0;
    } else if (current_length == MAXLEN) {
      printf("Current word exceeds maximum word length %d.\n", MAXLEN);
      return 1;
    } else {
      current_length++;
    }
  }
  if (current_length > 0) {
    if (current_length > longest_word) {
      longest_word = current_length;
    }
    histogram[current_length-1]++;
  }

  printf("Histogram by count\n");
  for (int h = highest_count; h > 0; h--) {
    for (int i = 0; i < longest_word; i++) {
      if (histogram[i] >= h) {
        printf("#");
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
  for (int i = 0; i < longest_word; i++) {
    printf("%d", i+1);
  }
  printf("\n");
  return 0;
}
