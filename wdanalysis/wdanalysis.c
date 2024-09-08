#include "wdanalysis.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../wdtranslator/wdtranslator.h"

static void print_line_str_as_bytes(FILE *const output, const char *str) {
  while (*(str++) != '\0') {
    if (*(str + 1) != '\0')
      fprintf(output, "%d ", *str);
    else
      fprintf(output, "%d\n", *str);
  }
}

static void print_strings_as_list(FILE *const output,
                                  const char *const *const strings,
                                  const size_t num_strings) {
  for (size_t i = 0; i < num_strings; i++) {
    const char *cur_str = strings[i];
    if (i < num_strings - 1)
      fprintf(output, "\"%s\", ", cur_str);
    else
      fprintf(output, "\"%s\"", cur_str);
  }
}

void print_all_wd(FILE *const output) {
  for (size_t i = 0; i < NUM_WINGDINGS; i++)
    fprintf(output, "%s\n", wingdings[i]);
  fflush(output);
}

void print_all_sorted_wd(FILE *const output) {
  for (size_t i = 0; i < NUM_WINGDINGS; i++)
    fprintf(output, "%s\n", sorted_wingdings[i]);
  fflush(output);
}

void print_wd_bytes(FILE *const output) {
  for (size_t i = 0; i < NUM_WINGDINGS; i++) {
    fprintf(output, "%s: ", wingdings[i]);
    print_line_str_as_bytes(output, wingdings[i]);
  }
  fflush(output);
}

void print_wd_bytes_as_array(FILE *const output) {
  fputc('{', output);
  print_strings_as_list(output, wingdings, NUM_WINGDINGS);
  fputs("}\n", output);
  fflush(output);
}

void print_sorted_wd_bytes(FILE *const output) {
  for (size_t i = 0; i < NUM_WINGDINGS; i++) {
    const char *cur_wd_char = sorted_wingdings[i];
    fprintf(output, "%s: ", cur_wd_char);
    print_line_str_as_bytes(output, cur_wd_char);
  }
  fflush(output);
}

void runtime_print_sorted_wd(FILE *const output) {
  char **wd_copy = copy_sorted_wingdings();
  for (size_t i = 0; i < NUM_WINGDINGS; i++)
    fprintf(output, "%s\n", wd_copy[i]);
  fflush(output);
  free(wd_copy);
}

void runtime_print_sorted_wd_bytes(FILE *const output) {
  char **wd_copy = copy_sorted_wingdings();
  for (size_t i = 0; i < NUM_WINGDINGS; i++) {
    const char *cur_wd_char = wd_copy[i];
    fprintf(output, "%s: ", cur_wd_char);
    print_line_str_as_bytes(output, cur_wd_char);
  }
  fflush(output);
  free(wd_copy);
}

static int cmp(const void *const a, const void *const b) {
  const char *const *str_a = a;
  const char *const *str_b = b;
  return strcmp(*str_a, *str_b);
}

char **copy_wingdings(void) {
  char **wd_copy = malloc(sizeof(wingdings));
  memcpy(wd_copy, wingdings, NUM_WINGDINGS);
  return wd_copy;
}

char **copy_sorted_wingdings(void) {
  char **wd_copy = malloc(sizeof(sorted_wingdings));
  memcpy(wd_copy, wingdings, sizeof(sorted_wingdings));
  qsort(wd_copy, NUM_WINGDINGS, MAX_WINGDINGS_SIZE, cmp);
  return wd_copy;
}

int main(void) {
  runtime_print_sorted_wd_bytes(fopen(SORTED_WD_OUTPUT_FILENAME, "w"));
  return 0;
}
