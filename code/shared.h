#ifndef __H_SHARED__
#define __H_SHARED__

typedef struct {
  int key;
  void *data;
} arr_el_t;

typedef int (*cmp)(const void *, const void *);

#endif // !__H_SHARED__
