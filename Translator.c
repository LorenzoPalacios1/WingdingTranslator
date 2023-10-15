#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "C-MyBasics\MyBasics.h"

#define NUM_WINGDINGS (sizeof(wingdings) / sizeof(*wingdings))

#define WINGDINGS_CHAR_MAX_SIZE (sizeof(*wingdings))
/*
 * This is the offset between a standard ASCII char's value and its corresponding Wingdings character.
 *
 * Subtract a char by this value when indexing wingdings[] to get its Wingdings counterpart, or add this value
 * to a valid index within wingdings[] to get that particular Wingdings' char equivalent.
 *
 * An example: Consider the char 'd', ASCII value 100. 'd' corresponds to '♎︎', index 68. If you only have 'd', you can
 * find its corresponding Wingdings via "wingdings['d' - ASCII_WINGDINGS_OFFSET]".
 */
#define ASCII_WINGDINGS_OFFSET (CHAR_MAX - NUM_WINGDINGS)

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

#define CASE_SENSITIVE_KEYWORDS 0

    // ASCII characters mapped to their respective Wingdings representation.
    static const char *const wingdings[] = {
        // Symbols 1 (!, ", #, $, %, &, ', (, ), *, +, ',' , -, ., /) (15 total)
        "✏︎", "✂︎", "✁︎", "👓︎", "🕭︎", "🕮︎", "🕯︎", "🕿︎", "✆︎", "🖂︎", "🖃︎", "📪︎",
        "📫︎", "📬︎", "📭︎",

        // Numerical characters (0-9) (10 total)
        "📁︎", "📂︎", "📄︎", "🗏︎", "🗐︎", "🗄︎", "⌛︎", "🖮︎", "🖰︎", "🖲︎",

        // Symbols 2 (:, ;, <, =, >, ?, @) (7 total)
        // '@' has no Wingdings equivalent - it's here only for the sake of compatibility
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

// By having one allocation (performed in main()), any usage of getStr() will thus not have to
// allocate memory itself. Instead, it can simply copy input into this memory block.
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

char wingdings_char_to_ascii_char(const char *_wingdings_char)
{
    for (int i = 0; i < (int)NUM_WINGDINGS; i++)
    {
        if (strcmp(_wingdings_char, wingdings[i]) == 0)
            return i + ASCII_WINGDINGS_OFFSET;
    }
    return '\0';
}

char *wingdings_to_ascii_str(const char *wingdings_to_translate)
{
    static char ascii_return[MAX_BYTE_READS + sizeof('\0')];

    size_t ascii_return_i = 0;
    for (; ascii_return_i < sizeof(ascii_return) - 1 && *wingdings_to_translate != '\0'; ascii_return_i++)
    {
        static char wingdings_container[WINGDINGS_CHAR_MAX_SIZE + sizeof('\0')];
        switch (*wingdings_to_translate)
        {
        // Anything with no valid Wingdings counterpart can just be thrown into the returned
        // array
        default:
            ascii_return[ascii_return_i] = *wingdings_to_translate;
            wingdings_to_translate++;
            break;
        // These cases comprise all of the possible first bytes within a single Wingdings "character".
        // In the case of -16, there are some Wingdings that have a different final byte as compared
        // to -30 and -32, hence why it does not flow downwards and has its additional checks.
        case -16:
        {
            // In this case, -80, -75, and the usual -114 can be the final byte for the edge case Wingdings "characters".
            // The former two final bytes also occur abnormally early; their respective parent Wingdings "character"
            // WILL contain 4 bytes compared to the usual 6 or 7.
            const char *last_byte = strchr(wingdings_to_translate, -80) || strchr(wingdings_to_translate, -75) ? wingdings_to_translate + 3 : strchr(wingdings_to_translate, -114);
            const size_t wingdings_char_size = last_byte - wingdings_to_translate;
            strncpy_s(wingdings_container, sizeof(wingdings_container), wingdings_to_translate, wingdings_char_size - sizeof('\0'));
            wingdings_container[wingdings_char_size + sizeof('\0')] = '\0';

            ascii_return[ascii_return_i] = wingdings_char_to_ascii_char(wingdings_container);

            wingdings_to_translate += wingdings_char_size;
            break;
        }
        case -30:
        case -32:
            break;
            printf("added: %d\n", ascii_return[ascii_return_i]);
        };
    }
    ascii_return[ascii_return_i] = '\0';
    printf("%s | %d chars\n", ascii_return, ascii_return_i);
    return ascii_return;
}

int check_if_str_is_keyword(const char *const str)
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
        fputs(convert_ascii_str_to_wingdings(input, INPUT_LEN), WINGDINGS_OUTPUT);
        fputc('\n', WINGDINGS_OUTPUT);
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
        "The selected translator is Wingdings-to-English\n"
        "Enter \"" EXIT_KEYWORD "\" to quit, or \"" CHANGE_TRANSLATOR_KEYWORD "\" to switch translators");

    FILE *wingdings_input_file = NULL;
    while (1)
    {
        // Using fputs() instead of puts() since puts() appends a newline, and I want the user's
        // input to be on the same line as the "Enter English here: " prompt.
        fputs("Enter the name of a file containing Wingdings: ", stdout);
        getStrStdin(&input, MAX_BYTE_READS);
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
                if (getStr(&input, '\n', MAX_BYTE_READS, wingdings_input_file) == 0)
                    fputs(wingdings_to_ascii_str(input), ENG_OUTPUT);
                fputc('\n', ENG_OUTPUT);
            } while (!feof(wingdings_input_file));
        }
        fclose(wingdings_input_file);
        puts("Done");
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
        fputs("Could not open output file (" WINGDINGS_TO_ENG_OUTPUT_FILENAME ")\n", stderr);
        return 0;
    }
    if (!WINGDINGS_OUTPUT)
    {
        fputs("Could not open output file (" ENG_TO_WINGDINGS_OUTPUT_FILENAME ")\n", stderr);
        return 0;
    }

    return 1;
}

/*
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
*/

int main(void)
{
    {
        const int are_files_valid = open_output_files();
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
