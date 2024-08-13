#include <stdio.h>
#include <string.h>

#include "wdtranslator.h"

#include "../C-MyBasics/strext/strext.h"

#define CHAR_TO_INT(c) ((c) - '0')

static int check_if_str_is_keyword(const string_t *const candidate) {
  if (strcmp(candidate->data, EXIT_KEYWORD) == 0) return EXIT_STATUS_CODE;
  if (strcmp(candidate->data, CHANGE_TRANSLATOR_KEYWORD) == 0)
    return CHANGE_TRANSLATOR_STATUS_CODE;
  return 0;
}

/*
 * Prompts the user for the translator they want to use.
 * This function will return either CODE_ENGLISH_TO_WINGDINGS, or
 * CODE_WINGDINGS_TO_ENGLISH.
 *
 * If the user inputs something invalid, the function will prompt the user again
 * until they input something valid.
 */
static void prompt_user_for_translator(void) {
  puts("\nWhat would you like to translate?");
  puts("0. Translate English-to-Wingdings");
  puts("1. Translate Wingdings-to-English");
// REWRITE
}

int main(void) {

  return 0;
}
