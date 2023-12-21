/*
 * This file contains a collection of functions relating to the analysis of the
 * Wingdings characters used by the translator.
 */

#ifndef _INC_WDANALYSIS
#define _INC_WDANALYSIS

#define WINGDINGS_OUTPUT_FILENAME "wingdings.txt"
#define SORTED_WINGDINGS_OUTPUT_FILENAME "sorted_wingdings.txt"
#define WINGDINGS_BYTES_OUTPUT_FILENAME "wingdings_bytes.txt"
#define WINGDINGS_BYTES_SORTED_OUTPUT_FILENAME "wingdings_bytes_sorted.txt"

static void print_wingdings(void);

static void print_wingdings_sorted(void);

static void print_wingdings_bytes(void);

static void print_wingdings_bytes_sorted(void);

int string_to_simple_hash(const char *str, const char op);

int *wingdings_as_hashes(const char op);

char **_sort_wingdings(void);

#endif
