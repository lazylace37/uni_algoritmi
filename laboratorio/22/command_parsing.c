#include "command_parsing.h"
#include <ctype.h>
#include <string.h>

command_t parse_command(input_iterator *it) {
  char buf[1024];
  size_t n = 0;
  while (it->input[it->i] != '\0' && !isspace(it->input[it->i])) {
    buf[n++] = it->input[it->i];
    it->i++;
  }

  if (strncmp(buf, "insert", 6) == 0) {
    return INSERT;
  } else if (strncmp(buf, "remove", 6) == 0) {
    return REMOVE;
  } else if (strncmp(buf, "find", 4) == 0) {
    return FIND;
  } else if (strncmp(buf, "clear", 5) == 0) {
    return CLEAR;
  } else if (strncmp(buf, "show", 4) == 0) {
    return SHOW;
  } else if (strncmp(buf, "height", 6) == 0) {
    return HEIGHT;
  }
  return EXIT;
}

int parse_get_number(input_iterator *it) {
  while (it->i < it->input_len && isspace(it->input[it->i])) {
    it->i++;
  }

  int number = 0;
  while (it->i < it->input_len && isdigit(it->input[it->i])) {
    number = number * 10 + it->input[it->i] - '0';
    it->i++;
  }
  return number;
}

void parse_get_string(input_iterator *it, char *buf, size_t capacity) {
  while (it->i < it->input_len && isspace(it->input[it->i])) {
    it->i++;
  }

  size_t n = 0;
  while (it->i < it->input_len && n < capacity) {
    buf[n++] = it->input[it->i];
    it->i++;
  }
  buf[n] = '\0';
}

size_t parse_numbers(int *arr, size_t capacity, input_iterator *it) {
  size_t n = 0;
  while (it->i < it->input_len && n < capacity) {
    if (isspace(it->input[it->i])) {
      it->i++;
      continue;
    }

    arr[n++] = parse_get_number(it);
  }

  return n;
}
