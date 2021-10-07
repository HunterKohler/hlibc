SHELL = bash

# -Wshadow
# -Wdouble-promotion

CFLAGS := \
	-std=c11 \
	-Wall \
	# -Wextra \
	-fanalyzer \
	-g

CPPFLAGS := -MD -MP -I./include

LDFLAGS :=
LDLIBS :=

SRC := $(shell find src -name *.c)
TEST := $(shell find tests -name *.c)

FILES := $(shell find . -regextype egrep -regex ".*\.[ch]([ch]|[xp+]{2})?$$")

SRC_OBJ := $(patsubst %.c,build/%.o,$(SRC))
TEST_OBJ := $(patsubst %.c,build/%.o,$(TEST)) $(SRC_OBJ)

OBJ := $(sort $(SRC_OBJ) $(TEST_OBJ))

TEST_BIN := bin/test

.PHONY: all clean lint format test
.ONESHELL:

all: $(TEST_BIN)

clean:
	@rm -rf bin build

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

headers:
	@./headers.py $(FILES)

test: $(TEST_BIN)
	@$(TEST_BIN)

$(TEST_BIN): $(TEST_OBJ)
	@mkdir -p $(@D)
	@$(CC) $(LDFLAGS) $(LDLIBS) $^ -o $@

$(OBJ): build/%.o : %.c
	@mkdir -p $(@D)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

-include $(shell find build -name *.d 2>/dev/null)