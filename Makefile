SHELL = bash

# -Wshadow
# -Wdouble-promotion
# -fanalyzer

CFLAGS := \
	-std=c11 \
	-Wall \
	-Wextra \
	-Wno-unused-function \
	-Wno-sign-compare

CPPFLAGS := -MD -MP -I./

LDFLAGS :=
LDLIBS :=

SRC := $(shell find hlibc -name \*.c)
TEST := $(shell find test -name \*.c)
MISC := $(shell find misc -name \*.c)

FILES := $(shell find . -regextype egrep -regex ".*\.[ch]([ch]|[xp+]{2})?$$")

SRC_OBJ := $(patsubst %.c,build/%.o,$(SRC))
MISC_OBJ := $(patsubst %.c,build/%.o,$(MISC))
TEST_OBJ := $(patsubst %.c,build/%.o,$(TEST))
OBJ := $(sort $(SRC_OBJ) $(MISC_OBJ) $(TEST_OBJ))

MISC_BIN := $(patsubst %.c,bin/%,$(MISC))
TEST_BIN := $(patsubst %.c,bin/%,$(TEST))
BIN := $(sort $(MISC_BIN) $(TEST_BIN))

.PHONY: all clean misc test

all: $(MISC_BIN) $(TEST_BIN)

clean:
	@rm -rf bin build

misc: $(MISC_BIN)

test: $(TEST_BIN)

$(BIN): bin/% : build/%.o
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@


$(OBJ): build/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


-include $(shell find build -name *.d 2>/dev/null)