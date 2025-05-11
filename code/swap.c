#include "swap.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* Swap SIZE bytes between addresses A and B.  These helpers are provided
   along the generic one as an optimization.  */

enum swap_type_t { SWAP_WORDS_64, SWAP_WORDS_32, SWAP_VOID_ARG, SWAP_BYTES };

static enum swap_type_t get_swap_type(void *const pbase, size_t size) {
  if ((size & (sizeof(uint32_t) - 1)) == 0 &&
      ((uintptr_t)pbase) % __alignof__(uint32_t) == 0) {
    if (size == sizeof(uint32_t))
      return SWAP_WORDS_32;
    else if (size == sizeof(uint64_t) &&
             ((uintptr_t)pbase) % __alignof__(uint64_t) == 0)
      return SWAP_WORDS_64;
  }
  return SWAP_BYTES;
}

static inline void swap_words_64(void *restrict _a, void *restrict _b,
                                 size_t n) {
  char *a = (char *)_a;
  char *b = (char *)_b;
  do {
    n -= 8;
    uint64_t t = *(uint64_t *)(a + n);
    *(uint64_t *)(a + n) = *(uint64_t *)(b + n);
    *(uint64_t *)(b + n) = t;
  } while (n);
}

static inline void swap_words_32(void *restrict _a, void *restrict _b,
                                 size_t n) {
  char *a = (char *)_a;
  char *b = (char *)_b;
  do {
    n -= 4;
    uint32_t t = *(uint32_t *)(a + n);
    *(uint32_t *)(a + n) = *(uint32_t *)(b + n);
    *(uint32_t *)(b + n) = t;
  } while (n);
}

static inline void memswap(void *restrict a, void *restrict b, size_t n) {
  /* Use multiple small memcpys with constant size to enable inlining on most
     targets.  */
  enum { SWAP_GENERIC_SIZE = 32 };
  unsigned char tmp[SWAP_GENERIC_SIZE];
  while (n > SWAP_GENERIC_SIZE) {
    memcpy(tmp, a, SWAP_GENERIC_SIZE);
    a = __mempcpy(a, b, SWAP_GENERIC_SIZE);
    b = __mempcpy(b, tmp, SWAP_GENERIC_SIZE);
    n -= SWAP_GENERIC_SIZE;
  }
  while (n > 0) {
    unsigned char t = ((unsigned char *)a)[--n];
    ((unsigned char *)a)[n] = ((unsigned char *)b)[n];
    ((unsigned char *)b)[n] = t;
  }
}

static void do_swap(void *restrict a, void *restrict b, size_t size,
                    enum swap_type_t swap_type) {
  if (swap_type == SWAP_WORDS_64)
    swap_words_64(a, b, size);
  else if (swap_type == SWAP_WORDS_32)
    swap_words_32(a, b, size);
  else
    memswap(a, b, size);
}

void swap(void *_a, void *_b, size_t size) {
  do_swap(_a, _b, size, get_swap_type(_a, size));
}
