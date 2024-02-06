/*
 * This file contains a collection of functions relating to the analysis of the
 * Wingdings characters used by the translator.
 */

#include <stdio.h>

#ifndef _INC_WDANALYSIS
#define _INC_WDANALYSIS

#define WINGDINGS_OUTPUT_FILENAME "wingdings.txt"
#define SORTED_WINGDINGS_OUTPUT_FILENAME "sorted_wingdings.txt"
#define WINGDINGS_BYTES_OUTPUT_FILENAME "wingdings_bytes.txt"
#define WINGDINGS_BYTES_SORTED_OUTPUT_FILENAME "wingdings_bytes_sorted.txt"

void print_wingdings(FILE *const output_stream, const char separator);

void print_wingdings_sorted(FILE *const output_stream, const char separator);

void print_wingdings_bytes(FILE *const output_stream, const char line_separator, const char byte_value_separator);

void print_wingdings_bytes_sorted(FILE *const output_stream, const char line_separator,
                                  const char byte_value_separator);

int string_to_simple_hash(const char *str, const char op);

int *wingdings_as_hashes(const char op);

char **_sort_wingdings(void);

#endif
