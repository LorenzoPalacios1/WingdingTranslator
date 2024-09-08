#include <stdio.h>

#include "../wdtranslator/wdtranslator.h"

void print_all_wingdings(FILE *const output) {
  for (size_t i = 0; i < NUM_WINGDINGS; i++) fprintf(output, "%s\n", wingdings[i]);
  fflush(output);
}

void print_all_sorted_wingdings(FILE *const output) {
  for (size_t i = 0; i < NUM_WINGDINGS; i++) fprintf(output, "%s\n", sorted_wingdings[i]);
  fflush(output);
}

void print_wingdings_bytes(FILE *const output) {
  for (size_t i = 0; i < NUM_WINGDINGS; i++) {
    const char *cur_wd_char = wingdings[i];
    while (*(cur_wd_char++) != '\0') {
      if (*(cur_wd_char + 1) != '\0')
        fprintf(output, "%d ", *cur_wd_char);
      else
        fprintf(output, "%d\n", *cur_wd_char);
    }
  }
  fflush(output);
}

void print_wingdings_bytes_sorted(FILE *const output) {
  for (size_t i = 0; i < NUM_WINGDINGS; i++) {
    const char *cur_wd_char = sorted_wingdings[i];
    fprintf(output, "%s: ", cur_wd_char);
    while (*(cur_wd_char++) != '\0') {
      if (*(cur_wd_char + 1) != '\0')
        fprintf(output, "%d ", *cur_wd_char);
      else
        fprintf(output, "%d\n", *cur_wd_char);
    }
  }
  fflush(output);
}
