/*
 * This file contains a collection of functions relating to the analysis of the
 * Wingdings characters used by the translator.
 */

#include "wdanalysis.h"
#include "../Translator.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * One byte is added in the expression
 * `...(WINGDINGS_CHAR_SIZE + 1)`
 * to account for the separator character.
 *
 * Then, the final byte (as highlighted with asterisks) in the expression
 * `...(WINGDINGS_CHAR_SIZE + 1) + *1*`
 * accounts for the null terminator.
 */
#define PRINT_WINGDINGS_BUF_SIZE (NUM_WINGDINGS * (WINGDINGS_CHAR_SIZE + 1) + 1)
/*
 * The base-10 logarithm of any number plus one is equal to the number of digits
 * in that number.
 */
#define PRINT_WINGDINGS_BYTES_BUF_SIZE (NUM_WINGDINGS * (WINGDINGS_CHAR_SIZE + 1) * ((size_t)log10(INT_MAX) + 1) + 1)

void print_wingdings(FILE *const output_stream, const char separator)
{
    char buffer[PRINT_WINGDINGS_BUF_SIZE];
    {
        size_t buffer_i = 0;
        for (size_t wingdings_i = 0; wingdings_i < NUM_WINGDINGS; wingdings_i++)
        {
            buffer_i += sprintf_s(buffer + buffer_i, sizeof(buffer) - buffer_i, "%s", wingdings[wingdings_i]);
            buffer[buffer_i++] = separator;
        }
        /*
         * Subtracting one(1) causes the null terminator to overwrite the
         * trailing separator character.
         */
        buffer[buffer_i - 1] = '\0';
    }
    fputs(buffer, output_stream);
}

void print_wingdings_sorted(FILE *const output_stream, const char separator)
{
    char buffer[PRINT_WINGDINGS_BUF_SIZE];
    {
        size_t buffer_i = 0;
        for (size_t wingdings_i = 0; wingdings_i < NUM_WINGDINGS; wingdings_i++)
        {
            buffer_i += sprintf_s(buffer + buffer_i, sizeof(buffer) - buffer_i, "%s", sorted_wingdings[wingdings_i]);
            buffer[buffer_i++] = separator;
        }
        buffer[buffer_i - 1] = '\0';
    }
    fputs(buffer, output_stream);
}

void print_wingdings_bytes(FILE *const output_stream, const char line_separator, const char byte_value_separator)
{
    char buffer[PRINT_WINGDINGS_BYTES_BUF_SIZE];
    {
        size_t buffer_i = 0;
        for (size_t wingdings_i = 0; wingdings_i < NUM_WINGDINGS; wingdings_i++)
        {
            const size_t REMAINING_BYTES_IN_BUFFER = sizeof(buffer) - buffer_i;
            const char *const current_wingdings = wingdings[wingdings_i];
            buffer_i += sprintf_s(buffer + buffer_i, REMAINING_BYTES_IN_BUFFER, "%s:", current_wingdings);
            for (size_t wd_byte_i = 0; current_wingdings[wd_byte_i] != '\0'; wd_byte_i++)
            {
                buffer[buffer_i++] = byte_value_separator;
                buffer_i += sprintf_s(buffer + buffer_i, REMAINING_BYTES_IN_BUFFER, "%d", current_wingdings[wd_byte_i]);
            }
            buffer[buffer_i++] = line_separator;
        }
        buffer[buffer_i - 1] = '\0';
    }
    fputs(buffer, output_stream);
}

void print_wingdings_bytes_sorted(FILE *const output_stream, const char line_separator, const char byte_value_separator)
{
    char buffer[PRINT_WINGDINGS_BYTES_BUF_SIZE];
    {
        size_t buffer_i = 0;
        for (size_t wingdings_i = 0; wingdings_i < NUM_WINGDINGS; wingdings_i++)
        {
            const size_t REMAINING_BYTES_IN_BUFFER = sizeof(buffer) - buffer_i;
            const char *const current_wingdings = sorted_wingdings[wingdings_i];
            buffer_i += sprintf_s(buffer + buffer_i, REMAINING_BYTES_IN_BUFFER, "%s:", current_wingdings);
            for (size_t wd_byte_i = 0; current_wingdings[wd_byte_i] != '\0'; wd_byte_i++)
            {
                buffer[buffer_i++] = byte_value_separator;
                buffer_i += sprintf_s(buffer + buffer_i, REMAINING_BYTES_IN_BUFFER, "%d", current_wingdings[wd_byte_i]);
            }
            buffer[buffer_i++] = line_separator;
        }
        buffer[buffer_i - 1] = '\0';
    }
    fputs(buffer, output_stream);
}

static int add(const int a, const int b)
{
    return a + b;
}

static int subtract(const int a, const int b)
{
    return a - b;
}

static int multiply(const int a, const int b)
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
    int (*const operation)(int, int) = (op == '+' ? add : op == '-' ? subtract : op == '*' ? multiply : add);

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
    int (*const operation)(int, int) = (op == '+' ? add : op == '-' ? subtract : op == '*' ? multiply : add);

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

static int cmp(const void *const a, const void *const b)
{
    const char *const *char_a = a;
    const char *const *char_b = b;
    return strcmp(*char_a, *char_b);
}

char **_sort_wingdings(void)
{
    static char wd_copy[NUM_WINGDINGS][sizeof(*wingdings)];

    for (size_t i = 0; i < NUM_WINGDINGS; i++)
        strcpy_s(wd_copy[i], sizeof(*wingdings), wingdings[i]);

    qsort(wd_copy, NUM_WINGDINGS, sizeof(*wingdings), cmp);
    return (char **)wd_copy;
}
