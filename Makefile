# Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
SHELL = bash

CPPFLAGS = -MD -MP -I./include
CFLAGS = \
	-fanalyzer \
	-fpic \
	-std=c11 \
	-Wall \
	-Wextra \
	-Wno-implicit-fallthrough \
	-Wno-sign-compare \
	-Wno-unused-function \
	-Wno-override-init \
	-Wno-analyzer-malloc-leak \
	-Wcast-align=strict
LDFLAGS = -L./build/hlibc -pthread
LDLIBS =

DEBUG ?= 1

ifeq ($(DEBUG), 1)
	OPTIMIZE ?= 0
	SANITIZE ?= address
	CPPFLAGS += -DDEBUG=1
	CFLAGS += -g3
	LDFLAGS +=
	LDLIBS +=
else
	OPTIMIZE ?= 3
	CPPFLAGS += -DDEBUG=0
	CFLAGS +=
	LDFLAGS +=
	LDLIBS +=
endif

CFLAGS += \
	$(if $(OPTIMIZE), -O$(OPTIMIZE)) \
	$(if $(SANITIZE), -fsanitize=$(SANITIZE)) \
	$(if $(MARCH), -march=$(MARCH))
LDFLAGS += \
	$(if $(OPTIMIZE), -O$(OPTIMIZE)) \
	$(if $(SANITIZE), -fsanitize=$(SANITIZE)) \
	$(if $(MARCH), -march=$(MARCH))

HLIBC_SRC = $(shell find hlibc -name \*.c)
HLIBC_OBJ = $(patsubst %.c,build/%.o,$(HLIBC_SRC))
HLIBC_SO = build/hlibc/hlibc.so

TEST_SRC = $(shell find test -name \*.c)
TEST_OBJ = $(patsubst %.c,build/%.o,$(TEST_SRC))
TEST_SO = build/test/test.so
TEST_BIN = bin/test

HTEST_SRC = $(shell find htest -name \*.c)
HTEST_OBJ = $(patsubst %.c,build/%.o,$(HTEST_SRC))
HTEST_SO = build/htest/htest.so

$(shell mkdir -p $(sort $(dir $(HLIBC_OBJ) $(HTEST_OBJ) $(TEST_OBJ) \
	 $(HLIBC_SO) $(HTEST_SO)  $(TEST_SO) $(TEST_BIN))))

.PHONY: all clean

all: $(HLIBC_OBJ) $(HTEST_OBJ) $(TEST_OBJ) $(HLIBC_SO) $(HTEST_SO) $(TEST_SO) \
	$(TEST_BIN)

clean:
	$(RM) -r bin build

$(HLIBC_SO): $(HLIBC_OBJ)
$(HTEST_SO): $(HTEST_OBJ) $(HLIBC_SO)
$(TEST_SO): $(TEST_OBJ) $(HTEST_SO) $(HLIBC_SO)

$(TEST_BIN): $(TEST_OBJ) $(HLIBC_OBJ) $(HTEST_OBJ)

hlibc: $(HLIBC_SO)
htest: $(HTEST_SO)

test: $(TEST_BIN)
	$(TEST_BIN)

build/%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build/%.so:
	$(CC) $(LDFLAGS) $(LDLIBS) -shared $^ -o $@

bin/%:
	$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@

-include $(shell find build -name \*.d 2>/dev/null)
