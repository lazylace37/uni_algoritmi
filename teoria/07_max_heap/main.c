#include "heap.h"
#include <assert.h>

static int is_max_heap(Heap *heap, size_t i) {
  size_t l = left(i);
  size_t r = right(i);
  int is_valid = 1;

  if (l < heap->heap_size) {
    is_valid = heap->H[i].key >= heap->H[l].key && is_max_heap(heap, l);
  }
  if (r < heap->heap_size) {
    is_valid &= heap->H[i].key >= heap->H[r].key && is_max_heap(heap, r);
  }
  return is_valid;
}

int main(void) {
  printf("\n=== Starting Heap Tests ===\n\n");

  // Test 1: Build Heap
  printf("Test 1: Building initial heap\n");
  Element elements[] = {{16}, {42}, {8},  {23}, {4},  {15}, {99},
                        {1},  {38}, {27}, {33}, {55}, {11}, {7},
                        {19}, {31}, {44}, {6},  {3},  {77}};
  size_t n = sizeof(elements) / sizeof(elements[0]);

  Heap *heap = build_heap(elements, n);
  assert(heap != NULL);
  assert(heap->heap_size == n);
  assert(is_max_heap(heap, 0));
  printf("Initial heap structure:\n");
  plot_heap_tree(heap);
  printf("\n");

  printf("\nTest 2: Inserting new strategic elements\n");
  Element new_elements[] = {
      {100}, // New maximum
      {50},  // Middle value
      {0}    // New minimum
  };

  // Test 2: Insert new elements with strategic values
  for (size_t i = 0; i < 3; i++) {
    printf("Inserting element with key: %d\n", new_elements[i].key);
    int insert_result = insert(heap, &new_elements[i]);
    assert(insert_result == 0);
    assert(is_max_heap(heap, 0));
    printf("Heap after insertion:\n");
    plot_heap_tree(heap);
    printf("\n");
  }

  // Test 3: Increase priority at different levels
  printf("\nTest 3: Testing priority increases at different heap levels\n");
  // Increase a leaf node to become root
  size_t leaf_index = heap->heap_size - 1;
  printf("Increasing key at index %zu from %d to 200\n", leaf_index,
         heap->H[leaf_index].key);
  increase_priority(heap, leaf_index, 200);
  assert(is_max_heap(heap, 0));
  printf("Heap after leaf increase:\n");
  plot_heap_tree(heap);
  printf("\n");

  // Test 4: Decrease priority at different levels
  printf("\nTest 4: Testing priority decreases throughout heap\n");
  // Decrease root
  printf("Decreasing root from %d to 30\n", heap->H[0].key);
  decrease_priority(heap, 0, 200 - 30);
  assert(is_max_heap(heap, 0));
  printf("Heap after root decrease:\n");
  plot_heap_tree(heap);
  printf("\n");

  // Test 5: Extract max sequence
  printf("\nTest 5: Extracting maximum elements sequence\n");
  for (int i = 0; i < 5; i++) {
    Element *max = extract_max(heap);
    assert(max != NULL);
    printf("Extracted max element %d: %d\n", i + 1, max->key);
    assert(is_max_heap(heap, 0));
    printf("Heap after extraction %d:\n", i + 1);
    plot_heap_tree(heap);
    printf("\n");
  }

  // Test 6: Mixed operations on remaining heap
  printf("\nTest 6: Mixed operations sequence\n");
  // Insert some new elements
  Element mixed_elements[] = {{45}, {67}, {12}, {89}, {25}};
  for (int i = 0; i < 5; i++) {
    printf("Inserting %d\n", mixed_elements[i].key);
    insert(heap, &mixed_elements[i]);
    printf("Heap after insertion:\n");
    plot_heap_tree(heap);
    printf("\n");

    if (i % 2 == 0) { // Every other insertion, do an extract
      Element *max = extract_max(heap);
      printf("Extracted max: %d\n", max->key);
      printf("Heap after extraction:\n");
      plot_heap_tree(heap);
      printf("\n");
    }
  }

  free(heap->H);
  free(heap);
  return 0;
}
