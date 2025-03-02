all: main bench system_clock_test

main: main.c insertion_sort.h insertion_sort.c merge_sort.h merge_sort.c allocator.h stdlib_allocator.c max_heap.h max_heap.c quick_sort.h quick_sort.c heap_sort.h heap_sort.c counting_sort.h counting_sort.c
	clang -Wall -Wextra -o main main.c insertion_sort.c merge_sort.c stdlib_allocator.c max_heap.c quick_sort.c heap_sort.c counting_sort.c

bench: bench.c counting_sort.h counting_sort.c merge_sort.h merge_sort.c allocator.h stdlib_allocator.c max_heap.h max_heap.c quick_sort.h quick_sort.c heap_sort.h heap_sort.c buffer_allocator.h buffer_allocator.c
	clang -Wall -Wextra -o bench bench.c counting_sort.c merge_sort.c stdlib_allocator.c max_heap.c quick_sort.c heap_sort.c buffer_allocator.c -lm

system_clock_test: system_clock_test.c
	clang -Wall -Wextra -o system_clock_test system_clock_test.c
