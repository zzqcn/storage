
ver = perf

CC = gcc
CFLAGS = -march=native

ifeq ($(ver), perf)
	CFLAGS += -O3 -DNDEBUG
else ifeq ($(ver), debug)
	CFLAGS += -O0 -g -DDEBUG
else
	$(error "invalid build type")
endif

default: all

all: simd_str strstr_bench

simd_str:
	$(CC) $(CFLAGS) -o $@ simd_str.c

strstr_bench: 
	$(CC) $(CFLAGS) -o $@ strstr_bench.c

clean:
	rm -rf simd_str strstr_bench

