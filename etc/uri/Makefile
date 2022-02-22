SHELL = bash

CFLAGS = -O3 -march=native
# CFLAGS =
CPPFLAGS = -I../include
LDFLAGS =
LDLIBS =

SRC = \
	./uri_jmp.c \
	./uri_switch.c \
	./uri.c \
	../hlibc/ctype.c \
	../hlibc/string.c \
	./build/uri.re.c

BIN = ./bin/uri

$(shell mkdir -p ./bin)
$(shell mkdir -p ./build)

.PHONY: clean

all: clean $(BIN)

clean:
	rm -f $(BIN)

./build/uri.re.c: uri.c.re
	re2c $< -o $@

$(BIN): $(SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $(BIN)
