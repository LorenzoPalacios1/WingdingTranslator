/*
 * This file contains a collection of functions relating to the analysis of
 * Wingdings characters used by the translator.
 */
#include <stdio.h>

#include "../wdtranslator/wdtranslator.h"

#define WINGDINGS_OUTPUT_FILENAME "wingdings.txt"
#define SORTED_WINGDINGS_OUTPUT_FILENAME "sorted_wingdings.txt"
#define WINGDINGS_BYTES_OUTPUT_FILENAME "wingdings_bytes.txt"
#define WINGDINGS_BYTES_SORTED_OUTPUT_FILENAME "wingdings_bytes_sorted.txt"

void print_all_wingdings(FILE *output);

void print_all_sorted_wingdings(FILE *output);

void print_wingdings_bytes(FILE *output);

void print_sorted_wingdings_bytes(FILE *output);
