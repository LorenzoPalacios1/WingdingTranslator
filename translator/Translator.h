#ifndef _INC_TRANSLATOR
#define _INC_TRANSLATOR

#include <stdbool.h>
#include <stdio.h>

#define NUM_WINGDINGS (sizeof(wingdings) / sizeof(*wingdings))

// The maximum size of a singular wingdings character.
#define WINGDINGS_MAX_SIZE (sizeof(*wingdings))

/*
 * This is the offset between a standard ASCII character value and its
 * corresponding Wingdings character.
 *
 * Subtract a character by this value when indexing `wingdings` to get its
 * Wingdings counterpart, or add this value to a valid index of wingdings[] to
 * get the indexed Wingdings's ASCII equivalent.
 *
 * Usage example: Consider the character 'd', ASCII value 100. 'd' corresponds
 * to the '♎︎' Wingdings symbol, index 68. Considering the value of 'd', its
 * corresponding Wingdings character will be:
 * - `wingdings['d' - ASCII_WINGDINGS_OFFSET]`
 */
#define ASCII_TO_WINGDINGS_OFFSET (char)(CHAR_MAX - NUM_WINGDINGS)

#define CODE_ENGLISH_TO_WINGDINGS (0)
#define CODE_WINGDINGS_TO_ENGLISH (1)

#define ENG_TO_WINGDINGS_OUTPUT_FILENAME "EnglishToWingdings.txt"

#define SHOULD_CLEAR_OUTPUT_FILES (true)

// Preferably keep the below keywords unique so they're less likely to interfere
// with normal input.
#define EXIT_KEYWORD "!exit"
#define EXIT_STATUS_CODE (10)

#define CHANGE_TRANSLATOR_KEYWORD "!chg"
#define CHANGE_TRANSLATOR_STATUS_CODE (11)

// ASCII characters mapped to their respective Wingdings representation.
static const char *const wingdings[] = {
    // Symbols 1 (!, ", #, $, %, &, ', (, ), *, +, ',' , -, ., /) (15 listed)
    "✏︎", "✂︎", "✁︎", "👓︎", "🕭︎", "🕮︎", "🕯︎", "🕿︎", "✆︎", "🖂︎", "🖃︎", "📪︎", "📫︎",
    "📬︎", "📭︎",

    // Numerical characters (0-9) (10 listed | 25 total Wingdings)
    "📁︎", "📂︎", "📄︎", "🗏︎", "🗐︎", "🗄︎", "⌛︎", "🖮︎", "🖰︎", "🖲︎",

    // Symbols 2 (:, ;, <, =, >, ?, @) (7 listed | 32 total Wingdings)
    // '@' has no Wingdings equivalent - it's here only for the sake of
    // compatibility
    "🖳︎", "🖴︎", "🖫︎", "🖬︎", "✇︎", "✍︎", "@",

    // Uppercase alphabetical characters (A-Z) (26 listed | 58 total Wingdings)
    "✌︎", "👌︎", "👍︎", "👎︎", "☜︎", "☞︎", "☝︎", "☟︎", "✋︎", "☺︎", "😐︎", "☹︎", "💣︎", "☠︎",
    "⚐︎", "🏱︎", "✈︎", "☼︎", "💧︎", "❄︎", "🕆︎", "✞︎", "🕈︎", "✠︎", "✡︎", "☪︎",

    // Symbols 3 ([, \, ], ^, _, `) (6 listed | 64 total Wingdings)
    "☯︎", "ॐ︎", "☸︎", "☸︎", "♈︎", "♉︎",

    // Lowercase alphabetical characters (a-z) (26 listed | 90 total Wingdings)
    "♋︎", "♌︎", "♍︎", "♎︎", "♏︎", "♐︎", "♑︎", "♒︎", "♓︎", "🙰", "🙵", "●︎", "❍︎",
    "■︎", "□︎", "◻︎", "❑︎", "❒︎", "⬧︎", "⧫︎", "◆︎", "❖︎", "⬥︎", "⌧︎", "⍓︎", "⌘︎",

    // Symbols 4 ({, |, }, ~) (4 listed | 94 total Wingdings)
    "❀︎", "✿︎", "❝︎", "❞︎"};

/*
 * Sorted with stdlib.h `qsort()` with a comparator function of:
 *
 * int cmp(const void *const a, const void *const b)
 * {
 *   const char *const *char_a = a;
 *   const char *const *char_b = b;
 *   return strcmp(*char_a, *char_b);
 * }
 */
static const char *const sorted_wingdings[] = {
    "@",  "ॐ︎",  "⌘︎",  "⌛︎", "⌧︎",  "⍓︎",  "■︎",  "□︎",  "◆︎",  "●︎",  "◻︎",  "☜︎",
    "☝︎",  "☞︎",  "☟︎",  "☠︎",  "☪︎",  "☯︎",  "☸︎",  "☸︎",  "☹︎",  "☺︎",  "☼︎",  "♈︎",
    "♉︎", "♋︎", "♌︎", "♍︎", "♎︎", "♏︎", "♐︎", "♑︎", "♒︎", "♓︎", "⚐︎",  "✁︎",
    "✂︎",  "✆︎",  "✇︎",  "✈︎",  "✋︎", "✌︎",  "✍︎",  "✏︎",  "✞︎",  "✠︎",  "✡︎",  "✿︎",
    "❀︎",  "❄︎",  "❍︎",  "❑︎",  "❒︎",  "❖︎",  "❝︎",  "❞︎",  "⧫︎",  "⬥︎",  "⬧︎",  "🏱︎",
    "👌︎", "👍︎", "👎︎", "👓︎", "💣︎", "💧︎", "📁︎", "📂︎", "📄︎", "📪︎", "📫︎", "📬︎",
    "📭︎", "🕆︎", "🕈︎", "🕭︎", "🕮︎", "🕯︎", "🕿︎", "🖂︎", "🖃︎", "🖫︎", "🖬︎", "🖮︎",
    "🖰︎", "🖲︎", "🖳︎", "🖴︎", "🗄︎", "🗏︎", "🗐︎", "😐︎", "🙰",  "🙵"};

/*
 * Since `sorted_wingdings` does not have the same indexing as `wingdings`, the
 * indexing trick with `ASCII_WINGDINGS_OFFSET` will not work.
 *
 * In light of this, below is a character array where each element corresponds
 * to `sorted_wingdings`.
 *
 * (I know ']' is repeated. It's probably a mistake I made early on when
 * compiling all the wingdings here.)
 */
static const char sorted_wd_to_ascii[] = {
    '@', '\\', 'z', '6', 'x', 'y', 'n', 'o',  'u', 'l', 'p', 'E', 'G', 'F',
    'H', 'N',  'Z', '[', ']', ']', 'L', 'J',  'R', '^', '_', 'a', 'b', 'c',
    'd', 'e',  'f', 'g', 'h', 'i', 'O', '#',  '"', ')', '>', 'Q', 'I', 'A',
    '?', '!',  'V', 'X', 'Y', '|', '{', 'T',  'm', 'q', 'r', 'v', '}', '~',
    't', 'w',  's', 'P', 'B', 'C', 'D', '$',  'M', 'S', '0', '1', '2', ',',
    '-', '.',  '/', 'U', 'W', '%', '&', '\'', '(', '*', '+', '<', '=', '7',
    '8', '9',  ':', ';', '5', '3', '4', 'K',  'j', 'k'};

char *ascii_str_to_wingdings(const char *ascii_str, const size_t ascii_strlen);

char wingdings_char_to_ascii_char(const char *_wingdings_char);

char *wingdings_to_ascii_str(const char *wingdings_to_translate);
#endif
