
ver = perf

CC = gcc
CFLAGS = 

ifeq ($(ver), perf)
	CFLAGS = -O3 -march=native -DNDEBUG
else ifeq ($(ver), debug)
	CFLAGS = -O0 -g -DDEBUG
else
	$(error "invalid build type")
endif

default: all

all: 
	#http-parser picohttpparser  picohttpparser-avx2
	$(CC) $(CFLAGS) -o http-parser-bench -Ihttp-parser/ http-parser/http_parser.c http-parser/bench.c
	$(CC) $(CFLAGS) -o picohttpparser-bench -Ipicohttpparser/ picohttpparser/picohttpparser.c picohttpparser/bench.c
	$(CC) $(CFLAGS) -o picohttpparser-avx2-bench -Ipicohttpparser-avx2/ picohttpparser-avx2/picohttpparser.c picohttpparser-avx2/bench.c

clean:
	rm -rf http-parser-bench picohttpparser-bench picohttpparser-avx2-bench

