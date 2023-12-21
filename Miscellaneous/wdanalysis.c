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

#ifndef _INC_WDANALYSIS
#include "wdanalysis.h"
#endif

// - Header Guards End -

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

/*
 * str - The string to be hashed.
 *
 * op - Determines the arithmetic operation to be carried out on the bytes
 * within str.
 *
 * Beware of potential overflow when using multiplication.
 */
int string_to_simple_hash(const char *str, const char op)
{
    int (*const operation)(int, int) = (op == '+' ? add : op == '-' ? subtract
                                                      : op == '*'   ? multiply
                                                                    : add);

    // The default value for hash_result is 0, which is suited for addition and
    // subtraction, but not multiplication. Therefore, if multiplication is being
    // done, hash_result must be initialized to 1 instead so as to not nullify
    // the values within the string.
    int hash_result = operation != multiply ? 0 : 1;
    for (; *str != '\0'; str++)
        hash_result = operation(hash_result, *str);

    return hash_result;
}

/*
 * op - Determines the arithmetic operation to be carried out the bytes
 * comprising each individual Wingdings character.
 *
 * This function always returns an int array of size NUM_WINGDINGS.
 */
int *wingdings_as_hashes(const char op)
{
    int (*const operation)(int, int) = (op == '+' ? add : op == '-' ? subtract
                                                      : op == '*'   ? multiply
                                                                    : add);

    static int hash_container[NUM_WINGDINGS];

    if (operation == multiply)
    {
        // My version of MinGW does not have memset_s(), and using memset() causes
        // a linter warning, so I opted for this instead.
        for (unsigned short i = 0; i < NUM_WINGDINGS; i++)
            hash_container[i] = 1;
    }

    for (size_t i = 0; i < NUM_WINGDINGS; i++)
    {
        const char *current_wd_char = wingdings[i];
        for (; *current_wd_char != '\0'; current_wd_char++)
            hash_container[i] = operation(hash_container[i], *current_wd_char);
    }
    return hash_container;
}
