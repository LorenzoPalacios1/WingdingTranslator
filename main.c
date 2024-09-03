#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "C-MyBasics/strext/strext.h"
#include "wdtranslator/wdtranslator.h"

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

static FILE *open_stream_from_user(const char *const mode) {
  char filepath_buf[FILEPATH_MAX_LENGTH];
  for (size_t i = 0; i < sizeof(filepath_buf); i++) {
    const char c = getchar();
    if (c == '\n' || c == EOF) break;
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

static void consume_line(FILE *const stream) { while (getc(stream) != '\n'); }

static action_code_t wd_to_ascii_translator(void) {
  string_t *wd_input = new_string(BASE_STR_CAPACITY);
  string_t *ascii_buf = new_string(BASE_STR_CAPACITY);
  while (1) {
    fputs("Enter a file containing Wingdings: ", stdout);
    FILE *const input_stream = open_stream_from_user("r");
    char c = getc(input_stream);
    for (; c != EOF; c = getc(input_stream))
      wd_input = append_char(wd_input, c);
    {
      const action_code_t code = is_keyword(wd_input->data);
      if (code != ACTION_CODE_NONE) {
        delete_string(wd_input);
        delete_string(ascii_buf);
        fclose(input_stream);
        return code;
      }
    }
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
static void prompt_user_for_translator(void) {
  while (1) {
    puts("\nChoose your translator:");
    puts("1. Translate ASCII-to-Wingdings");
    puts("2. Translate Wingdings-to-ASCII");
    char buf[2] = {getchar(), '\0'};
    const int user_selection = atoi(buf);
    if (buf[0] != '\n') consume_line(stdin);
    action_code_t return_status = ACTION_CODE_NONE;
    switch (user_selection) {
      case 1:
        return_status = ascii_to_wd_translator();
        break;
      case 2:
        return_status = wd_to_ascii_translator();
        break;
    }
    if (return_status == ACTION_CODE_EXIT) return;
  }
}

int main(void) {
  prompt_user_for_translator();
  return 0;
}
