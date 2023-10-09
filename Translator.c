#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "C-MyBasics\MyBasics.h"

#define NUM_WINGDINGS (sizeof(wingdings) / sizeof(*wingdings))

#define WINGDINGS_CHAR_MAX_SIZE (sizeof(*wingdings))

#define ENG_TO_WINGDINGS_OFFSET (CHAR_MAX - NUM_WINGDINGS)

#define CODE_ENGLISH_TO_WINGDINGS (0)
#define CODE_WINGDINGS_TO_ENGLISH (1)

#define WINGDINGS_TO_ENG_OUTPUT_FILENAME "WingdingsToEnglish.txt"
#define ENG_TO_WINGDINGS_OUTPUT_FILENAME "EnglishToWingdings.txt"

#define ENG_OUTPUT output_files[0]
#define WINGDINGS_OUTPUT output_files[1]

#define SHOULD_CLEAR_OUTPUT_FILES (1)

// The maximum number of bytes to read from one line of input.
#define MAX_BYTE_READS (1024)

// Preferably keep the below keywords unique enough so that they won't
// interfere with normal input when the translator picks up input.
// Regular words like "exit" could be legitimate user input for instance.

#define EXIT_KEYWORD "!exit"
#define EXIT_STATUS_CODE (10)

#define CHANGE_TRANSLATOR_KEYWORD "!chg"
#define CHANGE_TRANSLATOR_STATUS_CODE (11)

// ASCII characters mapped to their respective Wingdings representation.
static const char *const wingdings[] = {
    // Symbols 1 (!, ", #, $, %, &, ', (, ), *, +, ',' , -, ., /) (15 total)
    "✏︎", "✂︎", "✁︎", "👓︎", "🕭︎", "🕮︎", "🕯︎", "🕿︎", "✆︎", "🖂︎", "🖃︎", "📪︎",
    "📫︎", "📬︎", "📭︎",

    // Numerical characters (0-9) (10 total)
    "📁︎", "📂︎", "📄︎", "🗏︎", "🗐︎", "🗄︎", "⌛︎", "🖮︎", "🖰︎", "🖲︎",

    // Symbols 2 (:, ;, <, =, >, ?, @) (7 total)
    // '@' has no Wingdings equivalent; it's just here for the sake of compatibility
    "🖳︎", "🖴︎", "🖫︎", "🖬︎", "✇︎", "✍︎", "@",

    // Uppercase alphabetical characters (A-Z) (26 total)
    "✌︎", "👌︎", "👍︎", "👎︎", "☜︎", "☞︎", "☝︎", "☟︎", "✋︎", "☺︎", "😐︎", "☹︎",
    "💣︎", "☠︎", "⚐︎", "🏱︎", "✈︎", "☼︎", "💧︎", "❄︎", "🕆︎", "✞︎", "🕈︎", "✠︎",
    "✡︎", "☪︎",

    // Symbols 3 ([, \, ], ^, _, `) (6 total)
    "☯︎", "ॐ︎", "☸︎", "☸︎", "♈︎", "♉︎",

    // Lowercase alphabetical characters (a-z) (26 total)
    "♋︎", "♌︎", "♍︎", "♎︎", "♏︎", "♐︎", "♑︎", "♒︎", "♓︎", "🙰", "🙵", "●︎",
    "❍︎", "■︎", "□︎", "◻︎", "❑︎", "❒︎", "⬧︎", "⧫︎", "◆︎", "❖︎", "⬥︎", "⌧︎",
    "⍓︎", "⌘︎",

    // Symbols 4 ({, |, }, ~) (4 total)
    "❀︎", "✿︎", "❝︎", "❞︎"};

// output_files[0] is the ENGLISH OUTPUT FILE
// output_files[1] is the WINGDINGS OUTPUT FILE
static FILE *output_files[] = {NULL, NULL};

static char *input = NULL;

char *convert_ascii_str_to_wingdings(const char *const ascii_str, const size_t ascii_strlen)
{
    /*
     * The provided Wingdings array accounts for chars '!' (ASCII value 33) to '~' (ASCII value 126),
     * and nothing else.
     *
     * In other words, this means that the Wingdings equivalent for '!' is located at wingdings[0].
     * Thus, the number of valid Wingdings is equivalent to the number of ASCII characters
     * offset by '!' (33).
     *
     * So, using the array to translate an ASCII character to Wingdings requires you to use the
     * difference between the ASCII character's value and '!' (33).
     *
     * An example being the letter 'e' (value 101) which translates to '♏︎' (wingdings[68]) in Wingdings.
     * In order to get the Wingdings equivalent, you'd need to write something like the following:
     *
     * > "wingdings['e' - '!']"
     *
     * ...or...
     *
     * > "wingdings['e' - 33]"
     */
    static char buffer[MAX_BYTE_READS];
    size_t buffer_i = 0;
    for (size_t i = 0; i < ascii_strlen; i++)
    {
        const unsigned char current_char = ascii_str[i];
        if (current_char < ENG_TO_WINGDINGS_OFFSET)
            buffer[buffer_i++] = current_char;
        else
        {
            const char *const wingdings_char = wingdings[current_char - ENG_TO_WINGDINGS_OFFSET];
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

char *convert_wingdings_to_ascii(const char *const wingdings_to_translate)
{
    // The wingdings themselves are comprised of varying amounts of bytes, so instead I decided
    // to scan for specific starting bytes and ending bytes to help segment the input
    static char buffer[MAX_BYTE_READS];
    size_t i = 0;
    while (wingdings_to_translate[i] != '\0' && i < sizeof(buffer))
    {
        switch (wingdings_to_translate[i])
        {
        // These two starting bytes are guaranteed to have a one specific ending byte, so I can just let
        // them flow downwards
        case (char)-30:
        case (char)-16:
        {
            const char *wingdings_singleton_last_byte_pos = strchr(wingdings_to_translate, (char)-114);
            const size_t singleton_size = wingdings_to_translate - wingdings_singleton_last_byte_pos;
            strncpy_s(buffer + i, sizeof(buffer), wingdings_to_translate + i, singleton_size);
            i += singleton_size;
            puts("found");
            break;
        }
        case (char)-32:
        {
            const char *wingdings_singleton_last_byte_pos = strchr(wingdings_to_translate, (char)-114);
            const size_t singleton_size = wingdings_to_translate - wingdings_singleton_last_byte_pos;
            strncpy_s(buffer + i, sizeof(buffer), wingdings_to_translate + i, singleton_size);
            i += singleton_size;
            puts("found");
            break;
        }
            break;
        default:
            buffer[i] = wingdings_to_translate[i];
        }
    }

    return buffer;
}
inline int check_if_str_is_keyword(const char *const str)
{
    if (str == NULL)
        return -1;
    else if (strcasecmp(str, EXIT_KEYWORD) == 0)
        return EXIT_STATUS_CODE;
    else if (strcasecmp(str, CHANGE_TRANSLATOR_KEYWORD) == 0)
        return CHANGE_TRANSLATOR_STATUS_CODE;
    return 0;
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
int translate_eng_to_wingdings(void)
{
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
        const size_t INPUT_LEN = getStrStdin(&input, MAX_BYTE_READS) - 1;
        {
            const int is_keyword = check_if_str_is_keyword(input);
            if (is_keyword == -1)
                continue;
            if (is_keyword != 0)
                return is_keyword;
        }
        fputs(convert_ascii_str_to_wingdings(input, INPUT_LEN), ENG_OUTPUT);
        fputc('\n', ENG_OUTPUT);
    }
}

/*
 * - WORK IN PROGRESS -
 *
 * Container function for the translation loop
 * This function will prompt the user for Wingdings characters to be converted
 * into English characters, if possible.
 * If no equivalent exists, the entered character will remain in the output.
 *
 * If EXIT_KEYWORD or CHANGE_TRANSLATOR_KEYWORD are entered, this function will
 * terminate the translation loop and return the respective status code.
 */
int translate_wingdings_to_eng(void)
{
    puts(
        "The selected translator is Wingding-to-English\n"
        "Enter \"" EXIT_KEYWORD "\" to quit, or \"" CHANGE_TRANSLATOR_KEYWORD "\" to switch translators");
    while (1)
    {
        printf("Enter the name of a file containing Wingdings: ");
        const size_t INPUT_LEN = getStrStdin(&input, MAX_BYTE_READS);
        {
            const int is_keyword = check_if_str_is_keyword(input);
            if (is_keyword == -1)
                continue;
            if (is_keyword != 0)
                return is_keyword;
        }

        // We split the input into 8-byte keys as that's the largest possible Wingdings "character",
        // so we need to add some padding bytes if the number of bytes in the input is not divisible by 8
        {
            const size_t number_of_pad_bytes = INPUT_LEN % sizeof(*wingdings);
            for (size_t i = INPUT_LEN; i < INPUT_LEN + number_of_pad_bytes; i++)
                input[i] = '\0';
        }
    }
}
/*
 * Prompts the user for the translator they want to use.
 * This function will return either CODE_ENGLISH_TO_WINGDINGS, or CODE_WINGDINGS_TO_ENGLISH.
 *
 * If the user inputs something invalid, the function will prompt the user again until they input
 * something valid.
 */
int prompt_user_for_translator(void)
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

// Returns 1, true, if the output files are open.
// Returns 0 otherwise.
int open_output_files(void)
{
    if (SHOULD_CLEAR_OUTPUT_FILES)
    {
        fopen_s(&ENG_OUTPUT, WINGDINGS_TO_ENG_OUTPUT_FILENAME, "w");
        fopen_s(&WINGDINGS_OUTPUT, ENG_TO_WINGDINGS_OUTPUT_FILENAME, "w");
    }
    else
    {
        fopen_s(&ENG_OUTPUT, WINGDINGS_TO_ENG_OUTPUT_FILENAME, "a");
        fopen_s(&WINGDINGS_OUTPUT, ENG_TO_WINGDINGS_OUTPUT_FILENAME, "a");
    }

    if (!ENG_OUTPUT)
    {
        fputs("Could not open output file (" ENG_TO_WINGDINGS_OUTPUT_FILENAME ")\n", stderr);
        return 0;
    }
    if (!WINGDINGS_OUTPUT)
    {
        fputs("Could not open output file (" WINGDINGS_TO_ENG_OUTPUT_FILENAME ")\n", stderr);
        return 0;
    }

    return 1;
}

// Usually used to examine the bytes that comprise each Wingdings character.
void print_wingdings(void)
{
    for (size_t i = 0; i < NUM_WINGDINGS; i++)
    {
        const char *const item = wingdings[i];
        fputs(wingdings[i], WINGDINGS_OUTPUT);
        fputc(':', WINGDINGS_OUTPUT);
        fputc(' ', WINGDINGS_OUTPUT);

        const size_t item_len = strlen(item);
        switch ((unsigned long long)item[0])
        {
        case 0xffffffffffffffe2:
        case 0xfffffffffffffff0:
        case 0xffffffffffffffe0:
            fprintf(WINGDINGS_OUTPUT, "first byte: %d | last byte: %d  | length %llu",
                    (char)item[0], (char)item[item_len - 1], item_len);
        }
        fputc('\n', WINGDINGS_OUTPUT);
    }
}

int main(void)
{
    {
        const int are_files_valid = open_output_files();
        if (!are_files_valid)
            return are_files_valid;
    }
    print_wingdings();
    return 0;
    // "+ 1" to account for a null terminator
    input = malloc(MAX_BYTE_READS + 1);
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
