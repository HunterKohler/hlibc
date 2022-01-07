/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_TERM_H_
#define HLIBC_TERM_H_

#include <hlibc/def.h>

/*
 * ANSI Control Sequence Indicator
 *
 * Reference: https://en.wikipedia.org/wiki/ANSI_escape_code
 */
#define CSI "\x1b["

#define CURSOR_UP(n) CSI n "A"
#define CURSOR_DOWN(n) CSI n "B"
#define CURSOR_RIGHT(n) CSI n "C"
#define CURSOR_LEFT(n) CSI n "D"
#define CURSOR_NEXT_LINE(n) CSI n "E"
#define CURSOR_PREV_LINE(n) CSI n "F"
#define CURSOR_POSITION(n, m) CSI n ";" m "H"
#define ERASE_IN_DISPLAY(n) CSI n "J"
#define ERASE_IN_LINE(n) CSI n "K"
#define SCROLL_UP(n) CSI n "S"
#define SCROLL_DOWN(n) CSI n "T"
#define SGR(n) CSI n "m"

#define RESET SGR("0")
#define BOLD SGR("1")
#define FAINT SGR("2")
#define ITALIC SGR("3")
#define UNDERLINE SGR("4")
#define SLOW_BLINK SGR("5")
#define FAST_BLINK SGR("6")
#define REVERSE SGR("7")
#define HIDE SGR("8")
#define STRIKETHROUGH SGR("9")
#define DOUBLE_UNDERLINE SGR("21")
#define RESET_WEIGHT SGR("22")
#define RESET_ITALIC SGR("23")
#define RESET_UNDERLINE SGR("24")
#define RESET_BLINK SGR("25")
#define RESET_REVERSE SGR("27")
#define REVEAL SGR("28")
#define RESET_STRIKETHROUGH SGR("29")

#define FG_BLACK SGR("30")
#define FG_RED SGR("31")
#define FG_GREEN SGR("32")
#define FG_YELLOW SGR("33")
#define FG_BLUE SGR("34")
#define FG_MAGENTA SGR("35")
#define FG_CYAN SGR("36")
#define FG_WHITE SGR("37")
#define FG_8_BIT(n) SGR("38;5;" n "m")
#define FG_24_BIT(r, g, b) SGR("38;2;" r ";" g ";" b "m")
#define FG_DEFAULT SGR("39")

#define BG_BLACK SGR("40")
#define BG_RED SGR("41")
#define BG_GREEN SGR("42")
#define BG_YELLOW SGR("43")
#define BG_BLUE SGR("44")
#define BG_MAGENTA SGR("45")
#define BG_CYAN SGR("46")
#define BG_WHITE SGR("47")
#define BG_8_BIT(n) SGR("48;5;" n "m")
#define BG_24_BIT(r, g, b) SGR("48;2;" r ";" g ";" b "m")
#define BG_DEFAULT SGR("49")

#define FG_BRIGHT_BLACK SGR("90")
#define FG_BRIGHT_RED SGR("91")
#define FG_BRIGHT_GREEN SGR("92")
#define FG_BRIGHT_YELLOW SGR("93")
#define FG_BRIGHT_BLUE SGR("94")
#define FG_BRIGHT_MAGENTA SGR("95")
#define FG_BRIGHT_CYAN SGR("96")
#define FG_BRIGHT_WHITE SGR("97")

#define BG_BRIGHT_BLACK SGR("100")
#define BG_BRIGHT_RED SGR("101")
#define BG_BRIGHT_GREEN SGR("102")
#define BG_BRIGHT_YELLOW SGR("103")
#define BG_BRIGHT_BLUE SGR("104")
#define BG_BRIGHT_MAGENTA SGR("105")
#define BG_BRIGHT_CYAN SGR("106")
#define BG_BRIGHT_WHITE SGR("107")

// clang-format off
#define FORMAT_SPECIFIER(x)           \
    _Generic((x),                     \
        const char *: "%s",           \
        const signed char *: "%s",    \
        const unsigned char *: "%s",  \
        char *: "%s",                 \
        signed char *: "%s",          \
        unsigned char *: "%s",        \
                                      \
        char: "%c",                   \
        signed char: "%d",            \
                                      \
        short: "%d",                  \
        int: "%d",                    \
        long: "%ld",                  \
        long long: "%lld",            \
                                      \
        bool: "%u",                   \
        unsigned char: "%u",          \
        unsigned short: "%u",         \
        unsigned int: "%u",           \
        unsigned long: "%lu",         \
        unsigned long long: "%llu",   \
                                      \
        float: "%f",                  \
        double: "%f",                 \
        long double: "%Lf",           \
                                      \
        void *: "%p")
// clang-format on

#endif
