# ref: http://www.csheeet.com/en/latest/notes/c_make.html
apps = parser parser2 builder

all: $(apps)
.PHONY: all clean

parser_srcs  = $(wildcard cjson/*.c) parser.c
parser_objs = $(parser_srcs:.c=.o)

parser2_srcs  = $(wildcard cjson2/*.c) parser2.c
parser2_objs = $(parser2_srcs:.c=.o)

builder_srcs  = $(wildcard cjson/*.c) builder.c
builder_objs = $(builder_srcs:.c=.o)

objs = $(parser_objs) $(parser2_objs) $(builder_objs)
objs := $(sort $(objs))

CFLAGS = -Wall -g -O0
LDFLAGS =

parser: $(parser_objs)
	$(CC) $(CFLAGS) -I./cjson -o $@ $^ $(LDFLAGS)

parser2: $(parser2_objs)
	$(CC) $(CFLAGS) -I./cjson2 -o $@ $^ $(LDFLAGS)

builder: $(builder_objs)
	$(CC) $(CFLAGS) -I./cjson -o $@ $^ $(LDFLAGS)

$(objs): %.o:%.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(objs) $(apps)

