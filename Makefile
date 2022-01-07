SHELL = bash

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

ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0 -coverage
	LDFLAGS += -coverage
else
	CFLAGS += -O3
endif

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

.PHONY: all clean coverage

all: $(LIBS) $(OBJS) $(TESTS) $(BINS)
clean:
	$(RM) build bin coverage
remake: | clean all

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
