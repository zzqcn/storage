
CC=gcc
CFLAGS=-g -O3 -I$(RTE_SDK)/$(RTE_TARGET)/include
TARGET=test test_fast test_fast_avx2

default: $(TARGET)

test: test.c
	$(CC) -o $@ $(CFLAGS) $^

test_fast: test.c
	$(CC) -o $@ $(CFLAGS) -march=native -DFAST_MEMCMP $^

test_fast_avx2: test.c
	$(CC) -o $@ $(CFLAGS) -march=native -DRTE_MACHINE_CPUFLAG_AVX2 -DFAST_MEMCMP $^

clean:
	@rm -f $(TARGET)
