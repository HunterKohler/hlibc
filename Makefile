# Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
SHELL = bash

AR = /usr/bin/ar
RM = rm -rf
LCOV = $(shell which lcov)
GCOV = $(shell which gcov)
GENHTML = $(shell which genhtml)

CFLAGS = \
	-std=c11 \
	-Wall \
	-Wextra \
	-Wno-sign-compare \
	-Wno-override-init \
	-Wno-implicit-fallthrough \
	-Wno-unused-function \
	-fanalyzer \
	-pipe

CPPFLAGS = -MD -MP -I./include

LDFLAGS = -L./build/shared -pipe
LDLIBS = -pthread -lm

DEBUG ?= 1

ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0 -coverage -fsanitize=address -fno-inline \
		-fno-inline-small-functions -fno-default-inline
	LDFLAGS += -g -coverage -fsanitize=address
else
	CFLAGS += -O3
endif

SRC_HLIBC := $(shell find hlibc -name \*.c)
SRC_TESTLIB := $(shell find testlib -name \*.c)
SRC_TEST_HLIBC := $(shell find test/hlibc -name \*.c)

OBJ_HLIBC := $(patsubst %.c,build/%.o,$(SRC_HLIBC))
OBJ_TESTLIB := $(patsubst %.c,build/%.o,$(SRC_TESTLIB))
OBJ_TEST_HLIBC := $(patsubst %.c,build/%.o,$(SRC_TEST_HLIBC))

LIB_HLIBC = build/hlibc/hlibc.a
LIB_TESTLIB = build/testlib/testlib.a

BIN_TEST_HLIBC = bin/test/hlibc

LIBS = $(sort $(LIB_HLIBC) $(LIB_TESTLIB))
SRCS = $(sort $(SRC_HLIBC) $(SRC_TESTLIB) $(SRC_TEST_HLIBC))
OBJS = $(sort $(OBJ_HLIBC) $(OBJ_TESTLIB) $(OBJ_TEST_HLIBC))
BINS = $(sort $(BIN_TEST_HLIBC))

SHARED_OBJ_HLIBC := $(patsubst %.c,build/shared/%.o,$(SRC_HLIBC))
SHARED_LIB_HLIBC := build/shared/libhlibc.so

SHARED_OBJS := $(sort $(SHARED_OBJ_HLIBC))
SHARED_LIBS := $(sort $(SHARED_LIB_HLIBC))

.PHONY: all clean coverage

all: $(LIBS) $(OBJS) $(BINS) $(SHARED_LIBS) $(SHARED_OBJS) coverage

clean:
	$(RM) build bin coverage

remake: | clean all

COV_HLIBC_BASELINE = coverage/hlibc/baseline.info
COV_HLIBC_TEST = coverage/hlibc/test.info
COV_HLIBC_INFO = coverage/hlibc/lcov.info
COV_HLIBC_HTML = coverage/hlibc/html/index.html

coverage: $(COV_HLIBC_HTML)

$(COV_HLIBC_BASELINE): $(BIN_TEST_HLIBC)
	@mkdir -p $(@D)
	$(RM) $$(find build -name '*.gcda')
	$(LCOV) --config ./.lcovrc --gcov-tool $(GCOV) --capture --initial \
		--directory build/shared/hlibc --output-file $@

$(COV_HLIBC_TEST): $(COV_HLIBC_BASELINE) $(BIN_TEST_HLIBC)
	@mkdir -p $(@D)
	$(BIN_TEST_HLIBC) || true
	$(LCOV) --config ./.lcovrc --gcov-tool $(GCOV) --capture \
		--directory build/shared/hlibc --output-file $@

$(COV_HLIBC_INFO): $(COV_HLIBC_TEST) $(COV_HLIBC_BASELINE)
	@mkdir -p $(@D)
	$(LCOV) --config ./.lcovrc --gcov-tool $(GCOV) \
		$(addprefix --add-tracefile ,$^) --output-file $@

$(COV_HLIBC_HTML): $(COV_HLIBC_INFO)
	@mkdir -p $(@D)
	$(GENHTML) --config ./.lcovrc --output-directory coverage/hlibc/html $<

$(SHARED_OBJS): CFLAGS += -fpic
$(SHARED_OBJS): build/shared/%.o : %.c
$(OBJS): build/%.o : %.c
build/%.o:
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB_HLIBC): $(OBJ_HLIBC)
$(LIB_TESTLIB): $(OBJ_TESTLIB)
build/%.a:
	@mkdir -p $(@D)
	$(AR) -rcs $@ $^

$(SHARED_LIB_HLIBC): LDFLAGS += -shared
$(SHARED_LIB_HLIBC): $(SHARED_OBJ_HLIBC)

$(BIN_TEST_HLIBC): $(OBJ_TEST_HLIBC) $(LIB_TESTLIB) $(SHARED_LIB_HLIBC)

$(SHARED_LIBS): LDLIBS_PRIV =
$(BINS): LDLIBS_PRIV = -lhlibc

$(BINS) $(SHARED_LIBS):
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) $(LDLIBS) $(LDLIBS_PRIV) $(filter-out %.so,$^) -o $@


-include $(shell find build -name *.d 2>/dev/null)
