SHELL = bash

# -Wno-pointer-sign
# -Wno-sign-compare

CFLAGS := \
	-g \
	-O2 \
	-std=c11 \
	-fanalyzer \
	-Wall \
	-Wextra \
	-Wdouble-promotion \
	-Wno-sign-compare \
	-Wno-override-init \
	-Wno-unused-function

CPPFLAGS := -MD -MP -I./

LDFLAGS :=
LDLIBS := -pthread -lm

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

all: $(MISC_BIN) $(TEST_BIN) $(LIB_ARCHIVE)

clean:
	@rm -rf bin build

lib: $(LIB_ARCHIVE)
misc: $(MISC_BIN)
test: $(TEST_BIN)

run_tests: $(TEST_BIN)
	@printf "Running all tests:\n"
	@for i in $(TEST_BIN); do \
		printf "%s:\n" "$$i"; \
		$$i; \
	done

$(TEST_BIN): $(LIB_ARCHIVE)

$(BIN): bin/% : build/%.o
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@

$(OBJ): build/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(LIB_ARCHIVE): $(LIB_OBJ)
	ar -rcs $@ $^

-include $(shell find build -name *.d 2>/dev/null)