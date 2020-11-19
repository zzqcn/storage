
ver = perf

CC = gcc
CFLAGS = 
HS_INCLUDE = /usr/local/include/hs
HS_LIB = /hs/lib64

ifeq ($(ver), perf)
	CFLAGS = -O3 -march=native -DNDEBUG
else ifeq ($(ver), debug)
	CFLAGS = -O0 -g -DDEBUG
else
	$(error "invalid build type")
endif

default: all

all: http_parser_bench hs_bench hs_lite_bench pico_hs_bench pico_avx2_hs_bench

http_parser_bench:
	$(CC) $(CFLAGS) -o $@ http_parser.c http_parser_bench.c

hs_bench:
	$(CC) $(CFLAGS) -I$(HS_INCLUDE) -o $@ hs_bench.c -L$(HS_LIB) -lhs

hs_lite_bench:
	$(CC) $(CFLAGS) -I$(HS_INCLUDE) -o $@ hs_bench.c -L$(HS_LIB) -lhs

pico_hs_bench:
	$(CC) $(CFLAGS) -I$(HS_INCLUDE) -o $@ picohttpparser.c pico_hs_bench.c -L$(HS_LIB) -lhs

pico_avx2_hs_bench:
	$(CC) $(CFLAGS) -I$(HS_INCLUDE) -o $@ picohttpparser_avx2.c pico_avx2_hs_bench.c -L$(HS_LIB) -lhs

pico_zzq_hs_bench:
	$(CC) $(CFLAGS) -I$(HS_INCLUDE) -o $@ picohttpparser_zzq.c pico_zzq_hs_bench.c -L$(HS_LIB) -lhs

clean:
	rm -rf http_parser_bench hs_bench hs_lite_bench pico_hs_bench pico_avx2_hs_bench
