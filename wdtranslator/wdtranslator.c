#include "wdtranslator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../C-MyBasics/array/array.h"
#include "../C-MyBasics/randomgen/randomgen.h"
#include "../C-MyBasics/strext/strext.h"

string_t *ascii_char_to_wd_char(const char ascii_char) {
  if (ascii_char > ASCII_TO_WD_OFFSET)
    return new_string(wingdings[ascii_char - ASCII_TO_WD_OFFSET]);
  return NULL;
}

string_t *ascii_str_to_wd_str(const char *ascii_str, string_t *wd_output) {
  for (size_t i = 0; ascii_str[i] != '\0'; i++) {
    const char current_char = ascii_str[i];
    if (current_char > ASCII_TO_WD_OFFSET) {
      const char *const wd_char = wingdings[current_char - ASCII_TO_WD_OFFSET];
      wd_output = append_raw_str(wd_output, wd_char, strlen(wd_char));
    } else
      wd_output = append_char(wd_output, current_char);
  }
  return wd_output;
}

int search_sorted_wingdings(const char *const wd_char) {
  size_t min = 0, max = NUM_WINGDINGS;
  while (min < max) {
    const size_t mid = (min + max) / 2;
    const int cmp_result = strcmp(wd_char, sorted_wingdings[mid]);
    if (cmp_result < 0)
      max = mid;
    else if (cmp_result > 0)
      min = mid + 1;
    else
      return mid;
  }
  return -1;
}

int search_for_wd_candidate(const char *const wd_cand) {
  switch (*wd_cand) {
    /*
     * These cases comprise all of the possible first bytes within a Wingdings.
     * See the files within `wdanalysis/pre-generated` for details.
     */
    case -16:
    case -30:
    case -32: {
      /*
       * For most Wingdings, the final byte (shown as `int`) in their
       * representation will be `-114`. However, `-80` and `-75` can be
       * the final byte for two edge case Wingdings (🙰, 🙵). The aforementioned
       * Wingdings also have an abnormal length of `4` compared to the usual
       * `6` or `7`.
       */
      size_t wd_length;
      if (wd_cand[5] == WD_TERM)
        wd_length = 6;
      else if (wd_cand[6] == WD_TERM)
        wd_length = 7;
      else if (wd_cand[3] == WD_TERM_EXC_1 || wd_cand[3] == WD_TERM_EXC_2)
        wd_length = 4;
      else
        break;
      char wingdings_container[MAX_WINGDINGS_SIZE];
      strncpy(wingdings_container, wd_cand, wd_length);
      wingdings_container[wd_length] = '\0';
      return search_sorted_wingdings(wingdings_container);
    }
  }
  return -1;
}

char wd_char_to_ascii_char(const char *const wd_char) {
  const int wd_char_index = search_sorted_wingdings(wd_char);
  if (wd_char_index == -1) return '\0';
  return sorted_wd_to_ascii[wd_char_index];
}

string_t *wd_str_to_ascii_str(const char *wd_str, string_t *ascii_output) {
  while (*wd_str != '\0') {
    const int wd_index = search_for_wd_candidate(wd_str);
    if (wd_index == -1) {
      ascii_output = append_char(ascii_output, *wd_str);
      wd_str++;
      continue;
    }
    const char *const wd_char = sorted_wingdings[wd_index];
    const char ascii_char = sorted_wd_to_ascii[wd_index];
    ascii_output = append_char(ascii_output, ascii_char);
    wd_str += strlen(wd_char);
  }
  return ascii_output;
}
