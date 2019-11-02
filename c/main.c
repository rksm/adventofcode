#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void read_input() {
  char *right = 0;
  char *left = 0;
  size_t length;
  FILE *f = fopen("/home/robert/projects/2018-12_adventofcode/"
                  "2018-12_adventofcode/day1_rust/data/day5.txt",
                  "rb");

  if (f) {
    fseek(f, 0, SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);
    right = (char *)malloc(length);
    left = (char *)malloc(length);
    for (size_t i = 0; i < length; ++i) {
      left[i] = 'a';
    }

    if (right) {
      size_t written = fread(right, 1, length, f);
      if (written < length) {
        printf("error reading file\n");
        return;
      }
    }
    fclose(f);
  }

  clock_t start = clock();

  if (right) {
    size_t i = 0;
    size_t j = 0;
    left[i] = right[j];

    while (++j < length) {
      char l = left[i];
      char r = right[j];
      int reactive = l != r && toupper(l) == toupper(r);
      if (reactive) {
        if (i > 0) {
          i--;
        }
      } else {
        left[++i] = r;
      }
    }

    left[i + 1] = '\0';
    clock_t end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%zu %f\n", i, cpu_time_used);
    printf("%s\n", left);
  }

  return;
}

int main(int argc, char *argv[]) {
  printf("hello\n");
  read_input();
  return 0;
}
