#ifndef __H_HEAP_
#define __H_HEAP_

#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define HEAP_MAX_SIZE 256

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

typedef struct {
  int key;
} Element;

typedef struct {
  size_t length;
  size_t heap_size;
  Element *H;
} Heap;

static inline size_t left(size_t i) { return 2 * i + 1; }
static inline size_t right(size_t i) { return 2 * i + 1 + 1; }
static inline size_t parent(size_t i) { return ceil(i / 2.0 - 1); }

static inline void swap(Heap *heap, size_t i, size_t j) {
  Element temp = heap->H[i];
  heap->H[i] = heap->H[j];
  heap->H[j] = temp;
}

int increase_priority(Heap *heap, size_t i, int k);
void max_heapify(Heap *heap, size_t i);
int decrease_priority(Heap *heap, size_t i, int k);
Element *extract_max(Heap *heap);
int insert(Heap *heap, Element *el);
Heap *build_heap(Element elements[], size_t n);

void plot_heap_tree(Heap *heap);
void plot_heap_tree_vertical(Heap *heap, size_t idx, int indent);

#endif /* ifndef __H_HEAP_ */
