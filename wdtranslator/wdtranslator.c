#include "wdtranslator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../C-MyBasics/array/array.h"
#include "../C-MyBasics/randomgen/randomgen.h"
#include "../C-MyBasics/strext/strext.h"

string_t *ascii_char_to_wd_char(const char ascii_char) {
  if (ascii_char > ASCII_TO_WINGDINGS_OFFSET)
    return new_string(wingdings[ascii_char - ASCII_TO_WINGDINGS_OFFSET]);
  return NULL;
}

string_t *ascii_str_to_wd_str(const char *ascii_str, string_t *wd_output) {
  for (size_t i = 0; ascii_str[i] != '\0'; i++) {
    const char current_char = ascii_str[i];
    if (current_char > ASCII_TO_WINGDINGS_OFFSET) {
      const char *const wd_char =
          wingdings[current_char - ASCII_TO_WINGDINGS_OFFSET];
      wd_output = append_raw_str(wd_output, wd_char, strlen(wd_char));
    } else
      wd_output = append_char(wd_output, current_char);
  }
  return wd_output;
}

char wd_char_to_ascii_char(const char *wd_char) {
  size_t min = 0, max = NUM_WINGDINGS;
  while (min < max) {
    if (min >= max) break;
    const size_t mid = (min + max) / 2;
    const int cmp_result = strcmp(wd_char, sorted_wingdings[mid]);
    if (cmp_result < 0)
      max = mid;
    else if (cmp_result > 0)
      min = mid + 1;
    else
      return sorted_wd_to_ascii[mid];
  }
  return '\0';
}

string_t *wd_str_to_ascii_str(const char *wd_str, string_t *ascii_output) {
  while (*wd_str != '\0') {
    switch (*wd_str) {
      // Anything without a valid Wingdings counterpart can be copied as is.
      default:
        ascii_output = append_char(ascii_output, *wd_str);
        wd_str++;
        break;
      // These cases comprise all of the possible first bytes within a single
      // instance of Wingdings.
      case -16:
      case -30:
      case -32: {
        /*
         * For most Wingdings "characters", the final byte in their string
         * representation will be -114. However, bytes -80 and -75 can be the
         * final byte for two edge case Wingdings "characters". The former two
         * bytes also occur abnormally early; their respective parent Wingdings
         * "character" WILL contain 4 bytes compared to the usual 6 or 7.
         */
        /* clang-format off */
        const char *const terminator_byte =
            (strchr(wd_str, -114) - wd_str + 1) / 2 == 3 ?
            strchr(wd_str, -114) :
            (strchr(wd_str + 3, -114) - wd_str + 1) / 2 == 3 ?
            strchr(wd_str + 3, -114) : wd_str + 3;
        /* clang-format on */
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
        char wingdings_container[MAX_WINGDINGS_SIZE];
        const ptrdiff_t wd_char_size = terminator_byte - wd_str + 1;
        strncpy_s(wingdings_container, sizeof(wingdings_container), wd_str,
                  wd_char_size);
        wingdings_container[wd_char_size] = '\0';

        ascii_output = append_char(ascii_output,
                                   wd_char_to_ascii_char(wingdings_container));

        // This fixes the problem detailed in issue #2 concerning the characters
        // "🙰" and "🙵" when they precede the character "♎︎".
        if (ascii_output->data[ascii_output->length - 1] == '\0') {
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
          strncpy_s(wingdings_container, sizeof(wingdings_container), wd_str,
                    4);
          wingdings_container[5] = '\0';

          ascii_output = append_char(
              ascii_output, wd_char_to_ascii_char(wingdings_container));
          wd_str += 4;
          break;
        }
        wd_str += wd_char_size;
        break;
      }
    }
  }
  return ascii_output;
}
