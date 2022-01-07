SHELL = bash

RUN_BINS := ./tools/run_bins.py

CFLAGS := \
	-std=c11 \
	-Wall \
	-Wextra \
	-Wno-sign-compare \
	-Wno-override-init \
	-Wno-implicit-fallthrough \
	-Wno-unused-function \
	-fanalyzer

CPPFLAGS := -MD -MP -I./
LDFLAGS :=
LDLIBS := -pthread -lm

DEBUG ?= 1

ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0 -coverage
	LDFLAGS += -coverage
else
	CFLAGS += -O3
endif

LIB_ARCHIVE := build/hlibc.a

LIB := $(shell find hlibc -name \*.c)
TEST := $(shell find test -name \*.c)
MISC := $(shell find misc -name \*.c)

LIB_OBJ := $(patsubst %.c,build/%.o,$(LIB))
MISC_OBJ := $(patsubst %.c,build/%.o,$(MISC))
TEST_OBJ := $(patsubst %.c,build/%.o,$(TEST))
OBJ := $(sort $(LIB_OBJ) $(MISC_OBJ) $(TEST_OBJ))

MISC_BIN := $(patsubst %.c,bin/%,$(MISC))
TEST_BIN := $(patsubst %.c,bin/%,$(TEST))
BIN := $(sort $(MISC_BIN) $(TEST_BIN))

.PHONY: all clean misc test run_tests

all: $(LIB_ARCHIVE) $(TEST_BIN) $(MISC_BIN)

clean:
	@rm -rf bin build

remake: | clean all

lib: $(LIB_ARCHIVE)
misc: $(MISC_BIN)
test: $(TEST_BIN)

run_tests: $(TEST_BIN)
	@$(RUN_BINS) $(TEST_BIN)

run_misc: $(MISC_BIN)
	@$(RUN_BINS) $(MISC_BIN)

$(BIN): bin/% : build/%.o $(LIB_ARCHIVE)
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@

$(OBJ): build/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB_ARCHIVE): $(LIB_OBJ)
	$(AR) -rcs $@ $^

-include $(shell find build -name *.d 2>/dev/null)
