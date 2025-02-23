#include "buffer_allocator.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  void *buffer;
  size_t offset;
  size_t capacity;
} buffer_state_t;

void *buffer_alloc(size_t size, void *state) {
  buffer_state_t *buffer_state = state;

  // TODO: alignment

  if (buffer_state->offset + size > buffer_state->capacity) {
    fprintf(stderr, "Buffer Allocator: Buffer overflow\n");
    return NULL;
  }

  void *ptr = buffer_state->buffer + buffer_state->offset;
  buffer_state->offset += size;
  return ptr;
}

void buffer_dealloc(void *ptr, void *state) {
  (void)ptr;
  (void)state;
}

allocator_t buffer_allocator_init(size_t size) {
  buffer_state_t *state = malloc(sizeof(buffer_state_t));
  state->buffer = malloc(size);
  state->offset = 0;
  state->capacity = size;

  return (allocator_t){buffer_alloc, buffer_dealloc, state};
}

void buffer_allocator_fini(allocator_t allocator) {
  buffer_state_t *state = allocator.state;
  free(state->buffer);
  free(state);
}
