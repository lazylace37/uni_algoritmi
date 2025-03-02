#include "min_heap.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { EXIT, BUILD, LENGTH, GETMIN, EXTRACT, INSERT, CHANGE } command_t;

typedef struct {
  char *input;
  size_t i;
  size_t input_len;
} input_iterator;

command_t parse_command(input_iterator *it) {
  char buf[1024];
  size_t n = 0;
  while (it->input[it->i] != '\0' && !isspace(it->input[it->i])) {
    buf[n++] = it->input[it->i];
    it->i++;
  }

  if (strncmp(buf, "build", 5) == 0) {
    return BUILD;
  } else if (strncmp(buf, "length", 6) == 0) {
    return LENGTH;
  } else if (strncmp(buf, "getmin", 6) == 0) {
    return GETMIN;
  } else if (strncmp(buf, "extract", 7) == 0) {
    return EXTRACT;
  } else if (strncmp(buf, "insert", 6) == 0) {
    return INSERT;
  } else if (strncmp(buf, "change", 6) == 0) {
    return CHANGE;
  }
  return EXIT;
}

int get_number(input_iterator *it) {
  int number = 0;
  while (it->i < it->input_len && isdigit(it->input[it->i])) {
    number = number * 10 + it->input[it->i] - '0';
    it->i++;
  }
  return number;
}

size_t parse_numbers(int *arr, size_t capacity, input_iterator *it) {
  size_t n = 0;
  while (it->i < it->input_len && n < capacity) {
    if (isspace(it->input[it->i])) {
      it->i++;
      continue;
    }

    arr[n++] = get_number(it);
  }

  return n;
}

int main(void) {
  int *arr = malloc(1024 * sizeof(int));
  min_heap_t min_heap = {
      .data = arr,
      .length = 0,
      .capacity = 1024,
  };

  char buf[1024];
  char *read;
  while ((read = fgets(buf, sizeof(buf), stdin)) != NULL) {
    ssize_t len = strlen(read);
    if (len > 0 && read[len - 1] == '\n') {
      read[len - 1] = '\0';
      len--;
    }

    input_iterator it = {read, 0, len};
    command_t command = parse_command(&it);

    switch (command) {
    case BUILD: {
      int *arr = malloc(1024 * sizeof(int));
      size_t n = parse_numbers(arr, 1024, &it);
      min_heap = min_heap_build(arr, n, 1024);
      min_heap_print(&min_heap);
    } break;
    case LENGTH:
      printf("%zu\n", min_heap_length(&min_heap));
      min_heap_print(&min_heap);
      break;
    case GETMIN:
      printf("%d\n", min_heap_getmin(&min_heap));
      min_heap_print(&min_heap);
      break;
    case EXTRACT:
      min_heap_extract(&min_heap);
      min_heap_print(&min_heap);
      break;
    case INSERT: {
      int arr[1024];
      size_t n = parse_numbers(arr, 1024, &it);
      if (n < 1) {
        break;
      }

      int x = arr[0];
      min_heap_insert(&min_heap, x);
      min_heap_print(&min_heap);
    } break;
    case CHANGE: {
      int arr[1024];
      size_t n = parse_numbers(arr, 1024, &it);
      if (n < 2) {
        break;
      }

      size_t i = arr[0];
      int x = arr[1];
      min_heap_change(&min_heap, i, x);
      min_heap_print(&min_heap);
    } break;
    case EXIT:
      return 0;
    }
  }
  return 0;
}
