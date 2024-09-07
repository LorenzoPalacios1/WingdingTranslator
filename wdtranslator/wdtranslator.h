#ifndef _WD_TRANSLATOR
#define _WD_TRANSLATOR

#include <limits.h>
#include <stddef.h>

#include "../C-MyBasics/strext/strext.h"

/*
 * This is the offset between a standard ASCII character value and its
 * corresponding Wingdings.
 *
 * Subtract an ASCII character by this value when indexing `wingdings` to get
 * its Wingdings counterpart, or add this value to a valid index of `wingdings`
 * to get the indexed Wingdings's ASCII equivalent.
 *
 * ### Example:
 * Consider the ASCII character `'d'` whose ASCII value is `100`. `'d'`
 * corresponds to the '♎︎' Wingdings, which is stored at `wingdings[68]`.
 * The corresponding Wingdings for `'d'` can be algorithmically found within
 * `wingdings` like so:
 *
 * - `wingdings['d' - ASCII_TO_WD_OFFSET]`
 */
#define ASCII_TO_WD_OFFSET (char)(CHAR_MAX - NUM_WINGDINGS)

/*
 * The maximum size of a Wingdings stored within `wingdings`.
 * This includes padding bytes.
 */
#define MAX_WINGDINGS_SIZE (sizeof(*wingdings))

/* The number of elements in `wingdings`. */
#define NUM_WINGDINGS (sizeof(wingdings) / sizeof(*wingdings))

/* The common terminating byte for Wingdings. */
#define WD_TERM_VAL (-114)

static const char *const wingdings[] = {
    // Symbols 1 (!, ", #, $, %, &, ', (, ), *, +, ',' , -, ., /) (15 listed)
    "✏︎", "✂︎", "✁︎", "👓︎", "🕭︎", "🕮︎", "🕯︎", "🕿︎", "✆︎", "🖂︎", "🖃︎", "📪︎", "📫︎",
    "📬︎", "📭︎",

    // Numerical characters (0-9) (10 listed | 25 total Wingdings)
    "📁︎", "📂︎", "📄︎", "🗏︎", "🗐︎", "🗄︎", "⌛︎", "🖮︎", "🖰︎", "🖲︎",

    // Symbols 2 (:, ;, <, =, >, ?, @) (7 listed | 32 total Wingdings)
    // '@' has no Wingdings equivalent - it's here only as filler for
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
 * Sorted with `stdlib.h qsort()` with a comparator function of:
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

/*
 * Performs a binary search through sorted_wingdings to find the ASCII
 * equivalent for the given Wingdings.
 *
 * @return A `string_t` object containing the corresponding Wingdings for
 * `ascii_char`. If there is no corresponding Wingdings, `NULL` is returned.
 */
string_t *ascii_char_to_wd_char(char ascii_char);

string_t *ascii_str_to_wd_str(const char *ascii_str, string_t *wd_output);

int get_sorted_wd_char_index(const char *const wd_char);

string_t *wd_str_to_ascii_str(const char *wd_str, string_t *ascii_output);

char wd_char_to_ascii_char(const char *wd_char);

#endif
