#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "MyBasics.h"

#define NUM_WINGDINGS (sizeof(wingdings) / sizeof(*wingdings))

#define ENG_TO_WINGDINGS_OFFSET (CHAR_MAX - NUM_WINGDINGS)

#define CODE_ENGLISH_TO_WINGDINGS (0)
#define CODE_WINGDINGS_TO_ENGLISH (1)

#define WINGDINGS_TO_ENG_OUTPUT_FILENAME "WingdingsToEnglish.txt"
#define ENG_TO_WINGDINGS_OUTPUT_FILENAME "EnglishToWingdings.txt"

// The maximum number of bytes to read from one line of input.
#define MAX_BYTE_READS (1000)

// Preferably keep the below keywords unique enough so that they won't
// interfere with normal input when the translator picks up input.
// Regular words like "exit" could be legitimate user input for instance.

#define EXIT_KEYWORD "!exit"
#define EXIT_STATUS_CODE (10)

#define CHANGE_TRANSLATOR_KEYWORD "!chg"
#define CHANGE_TRANSLATOR_STATUS_CODE (11)

// ASCII characters mapped to their respective Wingdings representation.
static const char *wingdings[] = {
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

static FILE *english_output_file = NULL;
static FILE *wingdings_output_file = NULL;

static char *input = NULL;

inline int check_if_str_is_keyword(const char *str)
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
    printf(
        "The selected translator is English-to-Wingdings\n"
        "Enter \"%s\" to quit, or \"%s\" to switch translators\n",
        EXIT_KEYWORD, CHANGE_TRANSLATOR_KEYWORD);
    while (1)
    {
        printf("Enter English here: ");

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

        /*
         * The provided Wingdings array accounts for chars '!' (ASCII value 33) to '~' (ASCII value 126),
         * and nothing else.
         *
         * In other words, this means that the Wingdings equivalent for '!' is located at index 0, or
         * rather that the number of valid Wingdings is equivalent to the number of ASCII characters
         * offset by '!' (33).
         *
         * Thus, using the array to translate an ASCII character to Wingdings requires you to use the
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
        for (size_t i = 0; i < INPUT_LEN; i++)
        {
            const unsigned char current_char = input[i];
            if (current_char < ENG_TO_WINGDINGS_OFFSET)
            {
                fputc(current_char, english_output_file);
            }
            else
            {
                fprintf(english_output_file, wingdings[input[i] - ENG_TO_WINGDINGS_OFFSET]);
            }
        }
        fputc('\n', english_output_file);
        fflush(english_output_file);
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
int translate_wingdings_to_eng(void)
{
    printf(
        "The selected translator is Wingdings-to-English\n"
        "Enter \"%s\" to quit, or \"%s\" to switch translators\n",
        EXIT_KEYWORD, CHANGE_TRANSLATOR_KEYWORD);
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

        for (size_t i = 0; i < INPUT_LEN; i += sizeof(*wingdings))
        {
            const char *input_substr = input + i;

            for (size_t j = 0; j < NUM_WINGDINGS; j++)
            {
                const char *current_wingdings = wingdings[i];
                if (strncmp(current_wingdings, input_substr, strlen(current_wingdings)) == 0)
                {
                    puts("found");
                }
            }
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

// Returns 1, true, if the output files are open to somewhere
int ensure_validity_of_output_files(void)
{
    printf("%d\n", english_output_file->_flag);

    if (!english_output_file)
    {
        fprintf(stderr, "Could not open output file (%s)\n", ENG_TO_WINGDINGS_OUTPUT_FILENAME);
        return 0;
    }
    if (!wingdings_output_file)
    {
        fprintf(stderr, "Could not open output file (%s)\n", WINGDINGS_TO_ENG_OUTPUT_FILENAME);
        return 0;
    }
    return 1;
}

int main(void)
{
    fopen_s(&english_output_file, WINGDINGS_TO_ENG_OUTPUT_FILENAME, "a");
    fopen_s(&wingdings_output_file, ENG_TO_WINGDINGS_OUTPUT_FILENAME, "a");

    {
        const int are_files_valid = ensure_validity_of_output_files();
        if (!are_files_valid)
            return are_files_valid;
    }

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
