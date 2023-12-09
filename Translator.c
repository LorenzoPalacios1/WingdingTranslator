#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "C-MyBasics\MyBasics.h"
#include "Translator.h"

// By having one allocation (performed in main()), any usage of getStr() will thus not have to
// allocate memory itself. Instead, it can simply copy input into this memory block.
static char *input = NULL;

/* - Utility Functions - */

static inline int check_if_str_is_keyword(const char *const str)
{
    if (str == NULL)
        return -1;

    if (strcmp(str, EXIT_KEYWORD) == 0)
        return EXIT_STATUS_CODE;
    if (strcmp(str, CHANGE_TRANSLATOR_KEYWORD) == 0)
        return CHANGE_TRANSLATOR_STATUS_CODE;
    return 0;
}

/* - Functions to translate from ASCII to Wingdings and vice versa - */

char *ascii_str_to_wingdings(const char *const ascii_str, const size_t ascii_strlen)
{
    static char buffer[MAX_READABLE_BYTES];
    size_t buffer_i = 0;
    for (size_t i = 0; i < ascii_strlen; i++)
    {
        const char current_char = ascii_str[i];
        // For any exceptional characters that don't have a Wingdings counterpart, such as spaces
        if (current_char < ASCII_WINGDINGS_OFFSET)
            buffer[buffer_i++] = current_char;
        else
        {
            const char *const wingdings_char = wingdings[current_char - ASCII_WINGDINGS_OFFSET];
            const size_t wingdings_char_len = strlen(wingdings_char);
            strncpy_s(buffer + buffer_i, sizeof(buffer) - buffer_i, wingdings_char, wingdings_char_len);
            buffer_i += wingdings_char_len;
        }
        if (buffer_i >= sizeof(buffer))
        {
            buffer_i = sizeof(buffer) - 1;
            break;
        }
    }

    buffer[buffer_i] = '\0';
    return buffer;
}

// Performs a binary search through sorted_wingdings to find the ASCII equivalent for the given
// Wingdings.
char wingdings_char_to_ascii_char(const char *const _wingdings_char)
{
    for (int min = 0, max = NUM_WINGDINGS; min < max;)
    {
        if (min >= max)
            break;

        const int mid = (min + max) / 2;
        const int strcmp_result = strcmp(_wingdings_char, sorted_wingdings[mid]);
        if (strcmp_result == 0)
            return sorted_wd_to_ascii[mid];

        strcmp_result < 0 ? (max = mid) : (min = mid + 1);
    }

    return '\0';
}

char *wingdings_to_ascii_str(const char *wingdings_to_translate)
{
    static char ascii_return[MAX_READABLE_BYTES];
    size_t ascii_return_i = 0;
    for (; ascii_return_i < sizeof(ascii_return) - 1 && *wingdings_to_translate != '\0'; ascii_return_i++)
    {
        switch (*wingdings_to_translate)
        {
        // Anything without a valid Wingdings counterpart can just be thrown into the returned array
        default:
            ascii_return[ascii_return_i] = *wingdings_to_translate;
            wingdings_to_translate++;
            break;
        // These cases comprise all of the possible first bytes within a single Wingdings "character".
        case -16:
        case -30:
        case -32:
        {
            char wingdings_container[WINGDINGS_CHAR_MAX_SIZE];
            /*
             * For most Wingdings "characters", the final byte in their string representation will be -114.
             * However, bytes -80 and -75 can be the final byte for two edge case Wingdings "characters".
             * The former two bytes also occur abnormally early; their respective parent Wingdings "character"
             * WILL contain 4 bytes compared to the usual 6 or 7.
             */
            const char *const terminator_byte = (strchr(wingdings_to_translate, -114) - wingdings_to_translate + 1) / 2 == 3       ? strchr(wingdings_to_translate, -114)
                                                : (strchr(wingdings_to_translate + 3, -114) - wingdings_to_translate + 1) / 2 == 3 ? strchr(wingdings_to_translate + 3, -114)
                                                                                                                                   : wingdings_to_translate + 3;

            /* Yes, this bit is real obnoxious, but the problem is that some Wingdings "characters" have
             * -114 in the middle of their string representation AND at their end, so strchr() ends up
             * returning the byte that's in the middle of the Wingdings rather than the one at the end.
             *
             * To be honest, I'll probably end up forgetting what this even does in a day.
             * But hey, at least const-ness is preserved! That counts for something, right?
             *
             * (i'll see if i can make this less atrocious later)
             */

            const ptrdiff_t wingdings_char_size = terminator_byte - wingdings_to_translate + 1;
            strncpy_s(wingdings_container, sizeof(wingdings_container), wingdings_to_translate, wingdings_char_size);
            wingdings_container[wingdings_char_size] = '\0';

            ascii_return[ascii_return_i] = wingdings_char_to_ascii_char(wingdings_container);

            // This fixes the problem detailed in issue #2 concerning the characters "🙰" and "🙵"
            // when they precede the character "♎︎".
            if (ascii_return[ascii_return_i] == '\0'){
                /*
                 * Since the two aforementioned Wingdings characters corrupt "♎︎" and leave three
                 * of its last bytes in wingdings_to_translate, checking for the actual terminator
                 * byte of "♎︎" is possible. By identifying this terminator byte, the translator
                 * can backtrack and identify either one of the corrupting Wingdings characters
                 * and translate it appropriately.
                 */
                const char *const terminator_actual = strchr(wingdings_container + wingdings_char_size, -114);
                if (terminator_actual - wingdings_to_translate == 3){
                    // The constant (4) is equivalent to the length for the characters "🙰" and "🙵".
                    strncpy_s(wingdings_container, sizeof(wingdings_container), wingdings_to_translate, 4);
                    wingdings_container[5] = '\0';

                    ascii_return[ascii_return_i] = wingdings_char_to_ascii_char(wingdings_container);

                    wingdings_to_translate += 4;
                    break;
                }
            }

            wingdings_to_translate += wingdings_char_size;
            break;
        }
        }
    }
    ascii_return[ascii_return_i] = '\0';
    return ascii_return;
}

// Returns 0 if the output files were opened successfully.
// Returns an error code from fopen_s() otherwise.
static int open_output_files(void)
{
    errno_t error_code;
    if (SHOULD_CLEAR_OUTPUT_FILES)
    {
        error_code = fopen_s(&WINGDINGS_OUTPUT, ENG_TO_WINGDINGS_OUTPUT_FILENAME, "w");
    }
    else
    {
        error_code = fopen_s(&WINGDINGS_OUTPUT, ENG_TO_WINGDINGS_OUTPUT_FILENAME, "a");
    }

    if (!WINGDINGS_OUTPUT)
        fputs("Could not open output file (" ENG_TO_WINGDINGS_OUTPUT_FILENAME ")\n", stderr);

    return error_code;
}

/*
 * This function will prompt the user for English characters to be converted into
 * their respective Wingdings counterpart(s).
 *
 * If no equivalent exists, the entered character will remain in the output.
 *
 * If EXIT_KEYWORD or CHANGE_TRANSLATOR_KEYWORD are entered, this function will
 * terminate its translation loop and return the respective status code.
 */
static int translate_eng_to_wingdings(void)
{
    // i'll have to figure out a decent way to handle file opening, but for now this
    // ugly one-liner will do
    open_output_files();

    puts(
        "The selected translator is English-to-Wingdings\n"
        "Enter \"" EXIT_KEYWORD "\" to quit, or \"" CHANGE_TRANSLATOR_KEYWORD "\" to switch translators");
    while (1)
    {
        // Using fputs() instead of puts() since puts() appends a newline, and I want the user's
        // input to be on the same line as the "Enter English here: " prompt.
        fputs("Enter English here: ", stdout);

        // "- 1" since getStrStdin() returns the string's length including the null terminator,
        // which isn't needed in this case
        const size_t INPUT_LEN = getStrStdin(&input, MAX_READABLE_BYTES) - 1;
        {
            const int is_keyword = check_if_str_is_keyword(input);
            if (is_keyword == -1)
                continue;
            if (is_keyword != 0)
            {
                fclose(WINGDINGS_OUTPUT);
                return is_keyword;
            }
        }
        fputs(ascii_str_to_wingdings(input, INPUT_LEN), WINGDINGS_OUTPUT);
        fputc('\n', WINGDINGS_OUTPUT);
    }
}

/*
 * Container function for the translation loop
 * This function will prompt the user for Wingdings characters to be converted
 * into English characters, if possible.
 * If no equivalent exists, the entered character will remain in the output.
 *
 * If EXIT_KEYWORD or CHANGE_TRANSLATOR_KEYWORD are entered, this function will
 * terminate the translation loop and return the respective status code.
 */
static int translate_wingdings_to_eng(void)
{
    puts(
        "The selected translator is Wingdings-to-English\n"
        "Enter \"" EXIT_KEYWORD "\" to quit, or \"" CHANGE_TRANSLATOR_KEYWORD "\" to switch translators");

    FILE *wingdings_input_file = NULL;
    while (1)
    {
        // Using fputs() instead of puts() since puts() appends a newline, and I want the user's
        // input to be on the same line as the "Enter English here: " prompt.
        fputs("Enter the name of a file containing Wingdings: ", stdout);
        getStrStdin(&input, MAX_READABLE_BYTES);
        {
            const int is_keyword = check_if_str_is_keyword(input);
            if (is_keyword == -1)
                continue;
            if (is_keyword != 0)
                return is_keyword;
        }

        if (fopen_s(&wingdings_input_file, input, "r"))
            continue;

        {
            // If a pointer is non-null, getStr() assumes it points to allocated memory, and since there is an upper
            // bound to the length of input, we can just allocate that upper bound here rather than allocate and free
            // upon every new iteration
            do
            {
                // getStr() returns the length of the string it wrote, which isn't necessarily needed, but it returns 0
                // upon EOF or similar which can be used as a sort of "status code"
                if (getStr(&input, '\n', MAX_READABLE_BYTES, wingdings_input_file) != 0)
                    puts(wingdings_to_ascii_str(input));
            } while (!feof(wingdings_input_file));
        }
        fclose(wingdings_input_file);
    }
}

/*
 * Prompts the user for the translator they want to use.
 * This function will return either CODE_ENGLISH_TO_WINGDINGS, or CODE_WINGDINGS_TO_ENGLISH.
 *
 * If the user inputs something invalid, the function will prompt the user again until they input
 * something valid.
 */
static int prompt_user_for_translator(void)
{
    puts("\nWhat would you like to translate?");
    puts("0. Translate English-to-Wingdings");
    puts("1. Translate Wingdings-to-English");

    int user_choice;
    // Using a do-while instead of a generic while-loop to ensure that fseek() cleans up any
    // other unused input, especially newlines, since we only need one digit from the user
    // specifying which translator they're wanting
    do
    {
        user_choice = charToInt(getchar());
        fseek(stdin, 0, SEEK_END);
    } while (user_choice != CODE_ENGLISH_TO_WINGDINGS && user_choice != CODE_WINGDINGS_TO_ENGLISH);
    return user_choice;
}

int main(void)
{
    // "MAX_READABLE_BYTES + 1" to account for a null terminator
    input = malloc(MAX_READABLE_BYTES + 1);
    if (!input)
    {
        fputs("Could not allocate memory for input", stderr);
        return 2;
    }

    {
        puts("Welcome to the Wingdings \"translator\"!");
        int user_choice = prompt_user_for_translator();

        while (1)
        {
            const int return_status = user_choice ? translate_wingdings_to_eng() : translate_eng_to_wingdings();
            if (return_status == EXIT_STATUS_CODE)
                break;
            else if (return_status == CHANGE_TRANSLATOR_STATUS_CODE)
                user_choice = prompt_user_for_translator();
        }
    }
    _fcloseall();

    return 0;
}
