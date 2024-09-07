#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "C-MyBasics/strext/strext.h"
#include "wdtranslator/wdtranslator.h"

#define CODE_ASCII_TO_WD (1)
#define CODE_WD_TO_ASCII (2)

#define KEYWORD_EXIT "!exit"
#define KEYWORD_SWITCH "!chg"
#define ACTION_CODE_NONE (-1)
#define ACTION_CODE_EXIT (1)
#define ACTION_CODE_SWITCH (2)
#define KEYWORD_MAX_SIZE                                \
  (KEYWORD_EXIT > KEYWORD_SWITCH ? sizeof(KEYWORD_EXIT) \
                                 : sizeof(KEYWORD_SWITCH))

#define FILEPATH_MAX_LENGTH (4096)

typedef int translator_code_t;
typedef int action_code_t;

static action_code_t is_keyword(const char *const str) {
  if (strcmp(str, KEYWORD_EXIT) == 0) return ACTION_CODE_EXIT;
  if (strcmp(str, KEYWORD_SWITCH) == 0) return ACTION_CODE_SWITCH;
  return ACTION_CODE_NONE;
}

static FILE *open_stream_from_user(const char *const mode) {
  char filepath_buf[FILEPATH_MAX_LENGTH];
  for (size_t i = 0; i < sizeof(filepath_buf); i++) {
    const char c = getchar();
    if (c == '\n' || c == EOF) {
      filepath_buf[i] = '\0';
      break;
    }
    filepath_buf[i] = c;
  }
  return fopen(filepath_buf, mode);
}

static action_code_t ascii_to_wd_translator(void) {
  fputs("Enter an output path: ", stdout);
  FILE *const output_stream = open_stream_from_user("a");
  string_t *ascii_input = new_string(BASE_STR_CAPACITY);
  string_t *wd_buf = new_string(BASE_STR_CAPACITY);
  while (1) {
    fputs("Enter your ASCII text: ", stdout);
    for (char c = getchar(); c != '\n' && c != EOF; c = getchar())
      ascii_input = append_char(ascii_input, c);
    {
      const action_code_t code = is_keyword(ascii_input->data);
      if (code != ACTION_CODE_NONE) {
        delete_string(ascii_input);
        delete_string(wd_buf);
        fclose(output_stream);
        return code;
      }
    }
    wd_buf = ascii_str_to_wd_str(ascii_input->data, wd_buf);
    fputs(wd_buf->data, output_stream);
    fputc('\n', output_stream);
    fflush(output_stream);
    erase_string_contents(wd_buf);
    erase_string_contents(ascii_input);
  }
}

static inline void consume_line(FILE *const stream) {
  while (getc(stream) != '\n');
}

static action_code_t wd_to_ascii_translator(void) {
  string_t *wd_input = new_string(BASE_STR_CAPACITY);
  string_t *ascii_buf = new_string(BASE_STR_CAPACITY);
  while (1) {
    FILE *input_stream;
    {
      /* `open_stream_from_user()` is inlined here for keyword checking. */
      fputs("Enter a file containing Wingdings: ", stdout);
      char filepath_buf[FILEPATH_MAX_LENGTH];
      for (size_t i = 0; i < sizeof(filepath_buf); i++) {
        const char c = getchar();
        if (c == '\n' || c == EOF) {
          filepath_buf[i] = '\0';
          break;
        }
        filepath_buf[i] = c;
      }
      puts(filepath_buf);
      const action_code_t code = is_keyword(filepath_buf);
      if (code != ACTION_CODE_NONE) {
        delete_string(wd_input);
        delete_string(ascii_buf);
        return code;
      }
      input_stream = fopen(filepath_buf, "r");
      if (input_stream == NULL) {
        puts("The entered file or path was invalid.");
        continue;
      }
    }
    for (char c = getc(input_stream); c != EOF; c = getc(input_stream))
      wd_input = append_char(wd_input, c);
    ascii_buf = wd_str_to_ascii_str(wd_input->data, ascii_buf);
    puts(ascii_buf->data);
    erase_string_contents(ascii_buf);
    erase_string_contents(wd_input);
    fclose(input_stream);
  }
}

/*
 * Prompts the user for the translator they want to use.
 * This function will return either CODE_ENGLISH_TO_WINGDINGS, or
 * CODE_WINGDINGS_TO_ENGLISH.
 *
 * If the user inputs something invalid, the function will prompt the user again
 * until they input something valid.
 */
static translator_code_t prompt_for_translator(void) {
  while (1) {
    puts("\nChoose your translator:");
    puts("1. Translate ASCII-to-Wingdings");
    puts("2. Translate Wingdings-to-ASCII");
    char buf[] = {getchar(), '\0'};
    const translator_code_t user_selection = atoi(buf);
    if (buf[0] != '\n') consume_line(stdin);
    if (user_selection == CODE_ASCII_TO_WD ||
        user_selection == CODE_WD_TO_ASCII)
      return user_selection;
  }
}

int main(void) {
  action_code_t return_status = ACTION_CODE_NONE;
  do {
    const translator_code_t selection = prompt_for_translator();
    switch (selection) {
      case CODE_ASCII_TO_WD:
        return_status = ascii_to_wd_translator();
        break;
      case CODE_WD_TO_ASCII:
        return_status = wd_to_ascii_translator();
        break;
    }
  } while (return_status == ACTION_CODE_SWITCH);
  return 0;
}
