/*
 * This file contains a collection of functions relating to the analysis of
 * Wingdings characters used by the translator.
 */
#include <stdio.h>

#include "../wdtranslator/wdtranslator.h"

#define WD_OUTPUT_FILENAME "wingdings.txt"
#define SORTED_WD_OUTPUT_FILENAME "sorted_wingdings.txt"
#define WD_BYTES_OUTPUT_FILENAME "wingdings_bytes.txt"
#define SORTED_WD_BYTES_OUTPUT_FILENAME "wingdings_bytes_sorted.txt"

void print_all_wd(FILE *output);

void print_all_sorted_wd(FILE *output);

void print_wd_bytes(FILE *output);

void print_wd_bytes_as_array(FILE *output);

void runtime_print_sorted_wd(FILE *output);

void print_sorted_wd_bytes(FILE *output);

void runtime_print_sorted_wd_bytes(FILE *output);

char **copy_sorted_wingdings(void);
