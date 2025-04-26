#ifndef __H_ALLOCATOR__
#define __H_ALLOCATOR__
#include <stddef.h>

typedef struct {
  void *(*alloc)(size_t, void *state);
  void (*dealloc)(void *, void *state);
  void *state;
} allocator_t;

allocator_t get_default_allocator(void);

#endif // !__H_ALLOCATOR__
