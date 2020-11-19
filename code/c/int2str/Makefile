
ver = perf

CC = gcc
CFLAGS = -march=native

ifeq ($(ver), perf)
	CFLAGS += -O3 -g -DNDEBUG
else ifeq ($(ver), debug)
	CFLAGS += -O0 -g -DDEBUG
else
	$(error "invalid build type")
endif

all: int2str
default: all

SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

int2str: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf int2str $(OBJS)
