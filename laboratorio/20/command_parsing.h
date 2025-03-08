#ifndef __H_COMMAND_PARSING_
#define __H_COMMAND_PARSING_

#include <stddef.h>
typedef enum { EXIT, INSERT, REMOVE, FIND, CLEAR, SHOW } command_t;

typedef struct {
  char *input;
  size_t i;
  size_t input_len;
} input_iterator;

command_t parse_command(input_iterator *it);

int parse_get_number(input_iterator *it);

void parse_get_string(input_iterator *it, char *buf, size_t capacity);

size_t parse_numbers(int *arr, size_t capacity, input_iterator *it);

#endif // !__H_COMMAND_PARSING_
