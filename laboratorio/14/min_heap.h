#ifndef __H_MIN_HEAP_
#define __H_MIN_HEAP_
#include <stddef.h>

typedef struct {
  int *data;
  size_t length;
  size_t capacity;
} min_heap_t;

min_heap_t min_heap_build(int *arr, size_t n, size_t capacity);
size_t min_heap_length(min_heap_t *min_heap);
int min_heap_getmin(min_heap_t *min_heap);
int min_heap_extract(min_heap_t *min_heap);
void min_heap_insert(min_heap_t *min_heap, int el);
void min_heap_change(min_heap_t *min_heap, size_t i, int el);
void min_heap_print(min_heap_t *min_heap);
#endif // !__H_MIN_HEAP_
