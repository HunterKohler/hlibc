# Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
SHELL = bash

AR := /usr/bin/ar
RM := rm -rf

CFLAGS := \
	-std=c11 \
	-Wall \
	-Wextra \
	-Wno-sign-compare \
	-Wno-override-init \
	-Wno-implicit-fallthrough \
	-Wno-unused-function \
	-fanalyzer

CPPFLAGS := -MD -MP -I./include

LDFLAGS :=
LDLIBS := -pthread -lm

DEBUG ?= 1
SANATIZE ?= $(DEBUG)

ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0 -coverage -fsanitize=address
	LDFLAGS += -coverage -fsanitize=address
else
	CFLAGS += -O3
endif

LCOVFLAGS := \
	--no-external \
	--capture \
	--directory . \
	--output-file coverage/lcov.info

LIB_HLIBC := build/hlibc/hlibc.a
LIB_TESTLIB := build/testlib/testlib.a

SRC_HLIBC := $(shell find hlibc -name \*.c)
SRC_TESTLIB := $(shell find testlib -name \*.c)
SRC_TEST_HLIBC := $(shell find test/hlibc -name \*.c)

OBJ_HLIBC := $(patsubst %.c,build/%.o,$(SRC_HLIBC))
OBJ_TESTLIB := $(patsubst %.c,build/%.o,$(SRC_TESTLIB))
OBJ_TEST_HLIBC := $(patsubst %.c,build/%.o,$(SRC_TEST_HLIBC))

BIN_TEST_HLIBC := bin/test/hlibc

LIBS := $(sort $(LIB_HLIBC) $(LIB_TESTLIB))
SRCS := $(sort $(SRC_HLIBC) $(SRC_TESTLIB) $(SRC_TEST_HLIBC))
OBJS := $(sort $(OBJ_HLIBC) $(OBJ_TESTLIB) $(OBJ_TEST_HLIBC))
BINS := $(sort $(BIN_TEST_HLIBC))

.PHONY: all clean coverage test_hlibc

all: $(LIBS) $(OBJS) $(TESTS) $(BINS)

clean:
	$(RM) build bin coverage

remake: | clean all

test_hlibc: $(BIN_TEST_HLIBC)
	./$(BIN_TEST_HLIBC)

coverage: coverage/lcov.info coverage/html/index.html

build/tests_result: $(BIN_TEST_HLIBC)
	@mkdir -p $(@D)
	./$(BIN_TEST_HLIBC) ; echo $$? > $@

coverage/lcov.info: build/tests_result
	@mkdir -p $(@D)
	lcov $(LCOVFLAGS)

coverage/html/index.html: coverage/lcov.info
	@mkdir -p $(@D)
	genhtml --output-directory coverage/html coverage/lcov.info

view_coverage: coverage/html/index.html
	open $<

$(OBJS): build/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB_HLIBC): $(OBJ_HLIBC)
$(LIB_TESTLIB): $(OBJ_TESTLIB)
$(LIBS):
	@mkdir -p $(@D)
	$(AR) -rcs $@ $^

$(BIN_TEST_HLIBC): $(LIB_HLIBC) $(LIB_TESTLIB) $(OBJ_TEST_HLIBC)
$(BINS):
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@

-include $(shell find build -name *.d 2>/dev/null)
