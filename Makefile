# Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
SHELL = bash

AR = /usr/bin/ar
RM = rm -rf
MKDIR = mkdir -p
LCOV = $(shell which lcov)
GENHTML = $(shell which genhtml)

CPPFLAGS = -MD -MP -I./include
CFLAGS = -fanalyzer -fpic -pthread -std=c11 -Wall -Wextra \
	-Wno-implicit-fallthrough -Wno-override-init -Wno-sign-compare \
	-Wno-unused-function -Wno-analyzer-malloc-leak
LDFLAGS = -Lbuild/lib -pthread
LDLIBS =

DEBUG ?= 1

ifeq ($(DEBUG), 1)
	CPPFLAGS += -DDEBUG=1
	CFLAGS += -coverage -fno-inline -fsanitize=address -g3 -O0
	LDFLAGS += -coverage -fsanitize=address
	LDLIBS +=
else
	CPPFLAGS +=
	CFLAGS += -O3 -march=native
	LDFLAGS += -O3
	LDLIBS +=
endif

SRC_HLIBC = $(shell find hlibc -name \*.c)
SRC_HLIBC_TEST = $(shell find test/hlibc -name \*.c)
SRC_TESTCTL = $(shell find htest -name \*.c)

OBJ_HLIBC = $(patsubst %.c,build/%.o,$(SRC_HLIBC))
OBJ_HLIBC_TEST = $(patsubst %.c,build/%.o,$(SRC_HLIBC_TEST))
OBJ_TESTCTL = $(patsubst %.c,build/%.o,$(SRC_TESTCTL))

LIB_HLIBC = build/lib/libhlibc.so
LIB_TESTCTL = build/lib/libhtest.so

BIN_HLIBC_TEST = bin/test/hlibc

COV_HLIBC_INIT = coverage/hlibc/init.info
COV_HLIBC_TEST = coverage/hlibc/test.info
COV_HLIBC_INFO = coverage/hlibc/lcov.info
COV_HLIBC_HTML = coverage/hlibc/index.html

SRCS = $(SRC_HLIBC) $(SRC_TESTCTL) $(SRC_HLIBC_TEST)
OBJS = $(OBJ_HLIBC) $(OBJ_TESTCTL) $(OBJ_HLIBC_TEST)
LIBS = $(LIB_HLIBC) $(LIB_TESTCTL)
BINS = $(BIN_HLIBC_TEST)
COVS = $(COV_HLIBC_INIT) $(COV_HLIBC_TEST) $(COV_HLIBC_INFO) \
	$(COV_HLIBC_HTML)

ALLS = $(LIBS) $(OBJS) $(SRCS) $(BINS) $(COVS)
DIRS = $(sort $(dir $(ALLS)))

$(shell $(MKDIR) $(DIRS))

.PHONY: all clean remake coverage hlibc_test format

all: $(ALLS)

clean:
	$(RM) bin build coverage

remake: | clean all

hlibc_test: $(BIN_HLIBC_TEST)
	$(BIN_HLIBC_TEST)

format:
	@./scripts/format $(SRC) include

coverage: $(COVS)

build/%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

build/%.so:
	$(CC) $(LDFLAGS) $(LDLIBS) -shared $(filter-out %.so, $^) -o $@

bin/%:
	$(CC) $(LDFLAGS) $(LDLIBS) $(filter-out %.so, $^) -o $@

$(LIB_HLIBC): $(OBJ_HLIBC)
$(LIB_HLIBC): LDLIBS +=

$(LIB_TESTCTL): $(OBJ_TESTCTL) | $(LIB_HLIBC)
$(LIB_TESTCTL): LDLIBS += -lhlibc

$(BIN_HLIBC_TEST): $(OBJ_HLIBC_TEST) | $(LIB_HLIBC) $(LIB_TESTCTL)
$(BIN_HLIBC_TEST): LDLIBS += -lhlibc -lhtest

$(COV_HLIBC_INIT): $(LIB_HLIBC_TEST)
	$(RM) $$(find build -name '*.gcda')
	$(LCOV) -c -i -d build -o $@

$(COV_HLIBC_TEST): $(COV_HLIBC_INIT) $(BIN_HLIBC_TEST)
	$(BIN_HLIBC_TEST)
	$(LCOV) -c -d build -o $@

$(COV_HLIBC_INFO): $(COV_HLIBC_INIT) $(COV_HLIBC_TEST)
	$(LCOV) $(addprefix -a ,$^) -o $@

$(COV_HLIBC_HTML): $(COV_HLIBC_INFO)
	$(GENHTML) -o $(@D) $^

-include $(shell find build -name \*.d 2>/dev/null)
