CFLAGS := -O3 \
	-mcpu=ampere1a \
	-flto \
	-Wall \
	-Wextra \
	-Werror \
	-fno-common \
	-Winit-self \
	-Wundef \
	-Wshadow \
	-Wpointer-arith \
	-Wcast-align \
	-Wstrict-prototypes \
	-Wstrict-overflow=5 \
	-Wwrite-strings \
	-Waggregate-return \
	-Wcast-qual \
	-Wuninitialized \
	-Wswitch-default \
	-Wno-discarded-qualifiers \
	-Wno-aggregate-return

all: main bench system_clock_test

BENCH_SRCS = bench.c counting_sort.c stdlib_allocator.c heap.c quick_sort.c heap_sort.c buffer_allocator.c quick_sort_3_way.c swap.c
BENCH_OBJS = $(BENCH_SRCS:.c=.o)
BENCH_TARGET = bench
${BENCH_TARGET}: ${BENCH_OBJS}
	$(CC) ${BENCH_OBJS} -lm -o ${BENCH_TARGET}

MAIN_SRCS = main.c insertion_sort.c merge_sort.c stdlib_allocator.c heap.c quick_sort.c heap_sort.c counting_sort.c quick_sort_3_way.c swap.c
MAIN_OBJS = $(MAIN_SRCS:.c=.o)
MAIN_TARGET = main
$(MAIN_TARGET): $(MAIN_OBJS)
	$(CC) $(MAIN_OBJS) -o $(MAIN_TARGET)

system_clock_test: system_clock_test.c
	$(CC) $(CFLAGS) system_clock_test.c -o system_clock_test

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(BENCH_TARGET) $(MAIN_TARGET) system_clock_test

