#ifndef __H_BUFFER_ALLOCATOR_
#include "allocator.h"

allocator_t buffer_allocator_init(size_t size);
void buffer_allocator_fini(allocator_t allocator);

#endif // !__H_BUFFER_ALLOCATOR_
