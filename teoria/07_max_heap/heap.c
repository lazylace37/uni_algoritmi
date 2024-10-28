#include "heap.h"
#include <string.h>

int increase_priority(Heap *heap, size_t i, int k) {
  if (i >= heap->heap_size)
    return 1;

  heap->H[i].key += k;
  while (i > 0 && heap->H[i].key > heap->H[parent(i)].key) {
    swap(heap, i, parent(i));
    i = parent(i);
  }
  return 0;
}

void max_heapify(Heap *heap, size_t i) {
  size_t l = left(i);
  size_t r = right(i);
  size_t m = i;
  if (l < heap->heap_size && heap->H[l].key > heap->H[i].key) {
    m = l;
  }
  if (r < heap->heap_size && heap->H[r].key > heap->H[m].key) {
    m = r;
  }

  if (m != i) {
    swap(heap, m, i);
    max_heapify(heap, m);
  }
}

int decrease_priority(Heap *heap, size_t i, int k) {
  if (i >= heap->heap_size)
    return 1;

  heap->H[i].key -= k;
  max_heapify(heap, i);
  return 0;
}

Element *extract_max(Heap *heap) {
  if (heap->heap_size <= 0)
    return NULL;

  swap(heap, 0, heap->heap_size - 1);
  heap->heap_size--;
  if (heap->heap_size > 0) {
    max_heapify(heap, 0);
  }
  return &heap->H[heap->heap_size];
}

int insert(Heap *heap, Element *el) {
  if (heap->heap_size >= heap->length)
    return 1;

  heap->heap_size++;

  size_t i = heap->heap_size - 1;
  heap->H[i] = *el;

  while (i > 0 && heap->H[i].key > heap->H[parent(i)].key) {
    swap(heap, i, parent(i));
    i = parent(i);
  }
  return 0;
}

Heap *build_heap(Element elements[], size_t n) {
  Element *new_elements = malloc(HEAP_MAX_SIZE * sizeof(elements[0]));

  Heap *heap = malloc(sizeof(Heap));
  heap->length = HEAP_MAX_SIZE;
  heap->H = new_elements;
  heap->heap_size = 0;

  if (n > heap->length) {
    n = heap->length;
  }
  if (n > 0) {
    heap->heap_size = n;
    memcpy(new_elements, elements, n * sizeof(elements[0]));
  }

  for (int i = heap->heap_size / 2 - 1; i >= 0; i--) {
    max_heapify(heap, i);
  }
  return heap;
}

void plot_heap_tree(Heap *heap) {
  int levels = (int)(log2(heap->heap_size)) + 1;
  int index = 0;

  for (int i = 0; i < levels; i++) {
    int elementsInLevel = (1 << i);
    int spaces = (1 << (levels - i)) - 1;
    int spaceBetween = (1 << (levels - i + 1)) - 2;
    for (int s = 0; s < spaces; s++)
      printf(" ");

    for (int j = 0; j < elementsInLevel && index < (int)heap->heap_size;
         j++, index++) {
      printf("%02d", heap->H[index].key);
      for (int sb = 0; sb < spaceBetween; sb++)
        printf(" ");
    }
    printf("\n");
  }
}

void plot_heap_tree_vertical(Heap *heap, size_t idx, int indent) {
  if (idx >= heap->heap_size) {
    return;
  }
  Element el = heap->H[idx];
  printf("%*s%d\n", indent * 2, "", el.key);

  size_t l = left(idx);
  size_t r = right(idx);
  if (l < heap->heap_size) {
    plot_heap_tree_vertical(heap, l, indent + 1);
  }
  if (r < heap->heap_size) {
    plot_heap_tree_vertical(heap, r, indent + 1);
  }
}
