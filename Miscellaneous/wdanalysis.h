/*
 * This file contains a collection of functions relating to the analysis of the
 * Wingdings characters used by the translator.
 */

// - Header Guards Begin -

#ifndef _INC_STDIO
#include <stdio.h>
#endif

#ifndef _INC_TRANSLATOR
#include "../Translator.h"
#endif

// - Header Guards End -

#define WINGDINGS_OUTPUT_FILENAME "wingdings.txt"
#define SORTED_WINGDINGS_OUTPUT_FILENAME "sorted_wingdings.txt"
#define WINGDINGS_BYTES_OUTPUT_FILENAME "wingdings_bytes.txt"
#define WINGDINGS_BYTES_SORTED_OUTPUT_FILENAME "wingdings_bytes_sorted.txt"

void print_all_wingdings(void)
{
    FILE *out = fopen(WINGDINGS_OUTPUT_FILENAME, "w");
    for (size_t i = 0; i < NUM_WINGDINGS; i++)
        fprintf(out, "%s\n", wingdings[i]);

    fclose(out);
}

void print_all_wingdings_sorted(void)
{
    FILE *out = fopen(SORTED_WINGDINGS_OUTPUT_FILENAME, "w");

    for (size_t i = 0; i < NUM_WINGDINGS; i++)
        fprintf(out, "%s\n", sorted_wingdings[i]);

    fclose(out);
}

void print_wingdings_bytes(void)
{
    FILE *bytes_out = fopen(WINGDINGS_BYTES_OUTPUT_FILENAME, "w");

    for (size_t i = 0; i < NUM_WINGDINGS; i++)
    {
        fprintf(bytes_out, "%s: ", wingdings[i]);
        for (size_t j = 0;; j++)
        {
            if (wingdings[i][j] == '\0')
            {
                fputc('\n', bytes_out);
                break;
            }
            fprintf(bytes_out, "%d ", (int)wingdings[i][j]);
        }
    }
    fclose(bytes_out);
}

void print_wingdings_bytes_sorted(void)
{
    FILE *bytes_out = fopen(WINGDINGS_BYTES_SORTED_OUTPUT_FILENAME, "w");

    for (size_t i = 0; i < NUM_WINGDINGS; i++)
    {
        fprintf(bytes_out, "%s: ", sorted_wingdings[i]);
        for (size_t j = 0;; j++)
        {
            if (sorted_wingdings[i][j] == '\0')
            {
                fputc('\n', bytes_out);
                break;
            }
            fprintf(bytes_out, "%d ", (char)sorted_wingdings[i][j]);
        }
    }
    fclose(bytes_out);
}
