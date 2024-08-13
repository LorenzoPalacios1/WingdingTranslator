#include "wdtranslator.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../C-MyBasics/array/array.h"
#include "../C-MyBasics/randomgen/randomgen.h"
#include "../C-MyBasics/strext/strext.h"

string_t *ascii_str_to_wd_str(const string_t *const ascii_str) {
  const size_t ASCII_STR_LEN = ascii_str->length;
  string_t *wd_output = new_string(BASE_STR_CAPACITY);
  for (size_t i = 0; i < ASCII_STR_LEN; i++) {
    const char current_char = ascii_str->data[i];
    // For any characters that don't have a Wingdings counterpart, such as
    // breaks, spaces, or control characters.
    
    if (current_char < ASCII_TO_WINGDINGS_OFFSET)
      wd_output = append_char(wd_output, current_char);
    else {
      const char *const wd_char = wingdings[current_char - ASCII_TO_WINGDINGS_OFFSET];
      wd_output = append_raw_str(wd_output, wd_char);
    }
  }
  return wd_output;
}

// Performs a binary search through sorted_wingdings to find the ASCII
// equivalent for the given Wingdings.
char wd_char_to_ascii_char(const char *wd_char) {
  size_t min = 0, max = NUM_WINGDINGS;
  while (min < max) {
    if (min >= max) return '\0';
    const int mid = (min + max) / 2;
    const int strcmp_result = strcmp(wd_char, sorted_wingdings[mid]);
    if (strcmp_result == 0) return sorted_wd_to_ascii[mid];
    strcmp_result < 0 ? (max = mid) : (min = mid + 1);
  }
}

char *wingdings_to_ascii_str(const char *wingdings_to_translate) {
  static char ascii_return[TEMP_BUF_SIZE];
  size_t ascii_return_i = 0;
  // "sizeof(ascii_return) - 1" to account for a null terminator
  for (; ascii_return_i < sizeof(ascii_return) - 1 &&
         *wingdings_to_translate != '\0';
       ascii_return_i++) {
    switch (*wingdings_to_translate) {
      // Anything without a valid Wingdings counterpart can just be thrown into
      // the returned array
      default:
        ascii_return[ascii_return_i] = *wingdings_to_translate;
        wingdings_to_translate++;
        break;
      // These cases comprise all of the possible first bytes within a single
      // Wingdings "character".
      case -16:
      case -30:
      case -32: {
        char wingdings_container[MAX_WINGDINGS_SIZE];
        /*
         * For most Wingdings "characters", the final byte in their string
         * representation will be -114. However, bytes -80 and -75 can be the
         * final byte for two edge case Wingdings "characters". The former two
         * bytes also occur abnormally early; their respective parent Wingdings
         * "character" WILL contain 4 bytes compared to the usual 6 or 7.
         */
        const char *const terminator_byte =
            (strchr(wingdings_to_translate, -114) - wingdings_to_translate +
             1) / 2 ==
                    3
                ? strchr(wingdings_to_translate, -114)
            : (strchr(wingdings_to_translate + 3, -114) -
               wingdings_to_translate + 1) /
                        2 ==
                    3
                ? strchr(wingdings_to_translate + 3, -114)
                : wingdings_to_translate + 3;

        /* Yes, this bit is real ugly and hard to look at, but the problem is
         * that some Wingdings "characters" have -114 just smack-dab in the
         * middle of their string representation AND at their end, so strchr()
         * ends up returning the byte that's in the middle of the Wingdings
         * rather than the one at the end.
         *
         * To be honest, I'll probably end up forgetting what this even does in
         * a day. But hey, at least const-ness is preserved! That counts for
         * something, right?
         *
         * (i'll see if i can make this less atrocious later)
         */
        const ptrdiff_t wingdings_char_size =
            terminator_byte - wingdings_to_translate + 1;
        strncpy_s(wingdings_container, sizeof(wingdings_container),
                  wingdings_to_translate, wingdings_char_size);
        wingdings_container[wingdings_char_size] = '\0';

        ascii_return[ascii_return_i] =
            wd_char_to_ascii_char(wingdings_container);

        // This fixes the problem detailed in issue #2 concerning the characters
        // "🙰" and "🙵" when they precede the character "♎︎".
        if (ascii_return[ascii_return_i] == '\0') {
          /*
           * Since the two aforementioned Wingdings characters leave three of
           * the last bytes comprising "♎︎", checking for the actual terminator
           * byte of "♎︎" is possible. By identifying this terminator byte, the
           * translator can backtrack and identify either one of the preceding
           * problematic Wingdings characters and translate it appropriately.
           *
           * The constant (4) is equivalent to the length for the characters "🙰"
           * and "🙵".
           */
          strncpy_s(wingdings_container, sizeof(wingdings_container),
                    wingdings_to_translate, 4);
          wingdings_container[5] = '\0';

          ascii_return[ascii_return_i] =
              wd_char_to_ascii_char(wingdings_container);
          wingdings_to_translate += 4;
          break;
        }

        wingdings_to_translate += wingdings_char_size;
        break;
      }
    }
  }
  ascii_return[ascii_return_i] = '\0';
  return ascii_return;
}

/*
 * This function will prompt the user for English characters to be converted
 * into their respective Wingdings counterpart(s).
 *
 * If no equivalent exists, the entered character will remain unmodified in the
 * output retaining its relative position.
 *
 * If `EXIT_KEYWORD` or `CHANGE_TRANSLATOR_KEYWORD` are entered, this function
 * will terminate its translation loop and return the respective status code.
 */
char *translate_eng_to_wingdings(void) {}

/*
 * Container function for the translation loop
 * This function will prompt the user for Wingdings characters to be converted
 * into English characters, if possible.
 * If no equivalent exists, the entered character will remain in the output.
 *
 * If EXIT_KEYWORD or CHANGE_TRANSLATOR_KEYWORD are entered, this function will
 * terminate the translation loop and return the respective status code.
 */
char *translate_wingdings_to_eng(void) {}
