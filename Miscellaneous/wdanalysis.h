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

#define GENERIC_OUTPUT_FILENAME "out.txt"

#define WINGDINGS_OUTPUT_FILENAME "wingdings.txt"
#define SORTED_WINGDINGS_OUTPUT_FILENAME "sorted_wingdings.txt"
#define WINGDINGS_BYTES_OUTPUT_FILENAME "wingdings_bytes.txt"
#define WINGDINGS_BYTES_SORTED_OUTPUT_FILENAME "wingdings_bytes_sorted.txt"

void print_wingdings(void)
{
    FILE *const out = fopen(WINGDINGS_OUTPUT_FILENAME, "w");
    for (size_t i = 0; i < NUM_WINGDINGS; i++)
        fprintf(out, "%s\n", wingdings[i]);

    fclose(out);
}

void print_wingdings_sorted(void)
{
    FILE *const out = fopen(SORTED_WINGDINGS_OUTPUT_FILENAME, "w");

    for (size_t i = 0; i < NUM_WINGDINGS; i++)
        fprintf(out, "%s\n", sorted_wingdings[i]);

    fclose(out);
}

void print_wingdings_bytes(void)
{
    FILE *const bytes_out = fopen(WINGDINGS_BYTES_OUTPUT_FILENAME, "w");

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
    FILE *const bytes_out = fopen(WINGDINGS_BYTES_SORTED_OUTPUT_FILENAME, "w");

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

static inline int add(const int a, const int b)
{
    return a + b;
}

static inline int subtract(const int a, const int b)
{
    return a - b;
}

static inline int multiply(const int a, const int b)
{
    return a * b;
}

int string_to_simple_hash(const char *str, const char op)
{
    int (*const operation)(int, int) = (op == '+' ? add : op == '-' ? subtract
                                                        : op == '*' ? multiply
                                                                    : add);

    int hash_result = 0;
    while (*(str++) != '\0')
        hash_result = operation(hash_result, *str);

    return hash_result;
}

int *wingdings_as_hashes(const char op)
{
    int (*const operation)(int, int) = (op == '+' ? add : op == '-' ? subtract
                                                        : op == '*' ? multiply
                                                                    : add);

    static int hash_container[NUM_WINGDINGS];
    for (size_t i = 0; i < sizeof(hash_container); i++)
    {
        const char *current_wd_char = wingdings[i];
        while (*(current_wd_char++) != '\0')
            hash_container[i] = operation(hash_container[i], *current_wd_char);
    }
    return hash_container;
}
