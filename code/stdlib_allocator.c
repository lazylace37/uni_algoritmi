#include "allocator.h"
#include <stdlib.h>

void *default_alloc(size_t size, void *state) {
  (void)state;
  return malloc(size);
}

void default_dealloc(void *ptr, void *state) {
  (void)state;
  free(ptr);
}

allocator_t get_default_allocator() {
  return (allocator_t){.alloc = default_alloc, .dealloc = default_dealloc};
}
