all: main bench system_clock_test

main: main.c insertion_sort.h insertion_sort.c merge_sort.h merge_sort.c allocator.h stdlib_allocator.c heap.h heap.c quick_sort.h quick_sort.c heap_sort.h heap_sort.c counting_sort.h counting_sort.c quick_sort_3_way.h quick_sort_3_way.c
	clang -Wall -Wextra -o main main.c insertion_sort.c merge_sort.c stdlib_allocator.c heap.c quick_sort.c heap_sort.c counting_sort.c quick_sort_3_way.c

bench: bench.c counting_sort.h counting_sort.c merge_sort.h merge_sort.c allocator.h stdlib_allocator.c heap.h heap.c quick_sort.h quick_sort.c heap_sort.h heap_sort.c buffer_allocator.h buffer_allocator.c
	clang -Wall -Wextra -o bench bench.c counting_sort.c merge_sort.c stdlib_allocator.c heap.c quick_sort.c heap_sort.c buffer_allocator.c -lm

system_clock_test: system_clock_test.c
	clang -Wall -Wextra -o system_clock_test system_clock_test.c
