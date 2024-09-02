#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "C-MyBasics/strext/strext.h"
#include "translator/wdtranslator.h"

#define KEYWORD_EXIT "!exit"
#define KEYWORD_SWITCH "!chg"
#define ACTION_CODE_NONE (-1)
#define ACTION_CODE_EXIT (1)
#define ACTION_CODE_SWITCH (2)
#define KEYWORD_MAX_SIZE                                \
  (KEYWORD_EXIT > KEYWORD_SWITCH ? sizeof(KEYWORD_EXIT) \
                                 : sizeof(KEYWORD_SWITCH))

#define FILEPATH_MAX_LENGTH (4096)

typedef int action_code_t;

static action_code_t is_keyword(const char *const str) {
  if (strcmp(str, KEYWORD_EXIT) == 0) return ACTION_CODE_EXIT;
  if (strcmp(str, KEYWORD_SWITCH) == 0) return ACTION_CODE_SWITCH;
  return ACTION_CODE_NONE;
}

static FILE *open_append_stream_from_user(void) {
  char filepath_buf[FILEPATH_MAX_LENGTH];
  for (size_t i = 0; i < sizeof(filepath_buf); i++) {
    const char c = getchar();
    if (c == '\n' || c == EOF) break;
    filepath_buf[i] = c;
  }
  return fopen(filepath_buf, "a");
}

static action_code_t ascii_to_wd_translator(void) {
  fputs("Enter an output path: ", stdout);
  FILE *const output = open_append_stream_from_user();
  string_t *ascii_input = new_string(BASE_STR_CAPACITY);
  string_t *wd_output = new_string(BASE_STR_CAPACITY);
  while (1) {
    puts("Enter your ASCII text: ");
    {
      const action_code_t code = is_keyword(ascii_input->data);
      if (code != ACTION_CODE_NONE) {
        delete_string(ascii_input);
        fclose(output);
        return code;
      }
    }

    string_t *translated_str = ascii_str_to_wd_str(ascii_input->data, wd_output);
    fputs(translated_str->data, output);
    delete_string(translated_str);
  }
}

static void consume_line(FILE *const stream) { while (getc(stream) != '\n'); }

static action_code_t wd_to_ascii_translator(void) {}
/*
 * Prompts the user for the translator they want to use.
 * This function will return either CODE_ENGLISH_TO_WINGDINGS, or
 * CODE_WINGDINGS_TO_ENGLISH.
 *
 * If the user inputs something invalid, the function will prompt the user again
 * until they input something valid.
 */
static void prompt_user_for_translator(void) {
  puts("\nChoose your translator:");
  puts("1. Translate ASCII-to-Wingdings");
  puts("2. Translate Wingdings-to-ASCII");

  while (1) {
    char buf[2] = {getchar(), '\0'};
    const int user_selection = atoi(buf);
    consume_line(stdin);
    action_code_t return_status = ACTION_CODE_NONE;
    switch (user_selection) {
      case 1:
        return_status = ascii_to_wd_translator();
        break;
      case 2:
        return_status = wd_to_ascii_translator();
    }
    if (return_status == ACTION_CODE_EXIT) return;
  }
}

int main(void) {
  prompt_user_for_translator();
  return 0;
}
