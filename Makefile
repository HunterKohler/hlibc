SHELL = bash

# -Wshadow
# -Wdouble-promotion

CFLAGS := \
	-std=c11 \
	-Wall \
	-Wextra \
	-Wno-unused-function \
	-Wno-sign-compare \
	-fanalyzer

CPPFLAGS := -MD -MP -I./include

LDFLAGS :=
LDLIBS :=

SRC := $(shell find src -name \*.c)
TEST := $(shell find tests -name \*.c)

FILES := $(shell find . -regextype egrep -regex ".*\.[ch]([ch]|[xp+]{2})?$$")

GENERATED_HEADERS := ./include/hlibc/generated/pp.h

SRC_OBJ := $(patsubst %.c,build/%.o,$(SRC))
TEST_OBJ := $(patsubst %.c,build/%.o,$(TEST)) $(SRC_OBJ)

OBJ := $(sort $(SRC_OBJ) $(TEST_OBJ))

TEST_BIN := bin/test

.ONESHELL:

.PHONY: all
all: $(TEST_BIN)

.PHONY: clean
clean:
	@rm -rf bin build $(GENERATED_HEADERS)

.PHONY: lint
lint:
	@if ! cpplint --root=include $(FILES) ; then
		echo "cpplint found problems, exiting..."
		exit 1
	elif ! clang-format -n -Werror $(FILES) ; then
		echo "clang-format found problems, exiting..."
		exit 1
	elif ! ./headers.py $(FILES) ; then
		echo "headers found problems, exiting..."
		exit 1
	fi

.PHONY: format
format:
	@while true ; do
		read -p "Format all files in place? [y/n] " yn
		if [[ $$yn =~ ^[yY]([eE][sS])?$$ ]] ; then
			clang-format -i $(FILES)
			break
		elif [[ $$yn =~ ^[nN][oO]?$$ ]] ; then
			echo "Canceling format..."
			break
		fi
		printf "\e[1A\e[2K"
	done

.PHONY: test
test: $(TEST_BIN)
	@$(TEST_BIN)

$(TEST_BIN): $(TEST_OBJ)
	@mkdir -p $(@D)
	@$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@


$(OBJ): build/%.o : %.c $(GENERATED_HEADERS)
	@mkdir -p $(@D)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: generated
generated: $(GENERATED_HEADERS)

$(GENERATED_HEADERS): ./include/hlibc/generated/gen.py
	@./include/hlibc/generated/gen.py

-include $(shell find build -name *.d 2>/dev/null)