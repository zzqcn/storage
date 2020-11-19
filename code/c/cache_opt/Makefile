
targets=a b c d e f

CC=gcc
CFLAGS=-Wall -march=native -O3

all:
	@for i in `echo $(targets)`; do \
		$(CC) $(CFLAGS) -o $$i $$i.c; \
	done

clean:
	@for i in `echo $(targets)`; do \
		rm -rf $$i; \
	done
