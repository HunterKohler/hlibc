/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_TERM_H_
#define HLIBC_TERM_H_

/*
 * ANSI Control Sequence Indicator
 */
#define ANSI_CSI "\x1b["

#define ANSI_CURSOR_UP(n) ANSI_CSI n "A"
#define ANSI_CURSOR_DOWN(n) ANSI_CSI n "B"
#define ANSI_CURSOR_RIGHT(n) ANSI_CSI n "C"
#define ANSI_CURSOR_LEFT(n) ANSI_CSI n "D"
#define ANSI_CURSOR_NEXT_LINE(n) ANSI_CSI n "E"
#define ANSI_CURSOR_PREV_LINE(n) ANSI_CSI n "F"
#define ANSI_CURSOR_POSITION(n, m) ANSI_CSI n ";" m "H"
#define ANSI_ERASE_IN_DISPLAY(n) ANSI_CSI n "J"
#define ANSI_ERASE_IN_LINE(n) ANSI_CSI n "K"
#define ANSI_SCROLL_UP(n) ANSI_CSI n "S"
#define ANSI_SCROLL_DOWN(n) ANSI_CSI n "T"
#define ANSI_SGR(n) ANSI_CSI n "m"

#define ANSI_RESET ANSI_SGR("0")
#define ANSI_BOLD ANSI_SGR("1")
#define ANSI_FAINT ANSI_SGR("2")
#define ANSI_ITALIC ANSI_SGR("3")
#define ANSI_UNDERLINE ANSI_SGR("4")
#define ANSI_SLOW_BLINK ANSI_SGR("5")
#define ANSI_FAST_BLINK ANSI_SGR("6")
#define ANSI_REVERSE ANSI_SGR("7")
#define ANSI_HIDE ANSI_SGR("8")
#define ANSI_STRIKETHROUGH ANSI_SGR("9")
#define ANSI_DOUBLE_UNDERLINE ANSI_SGR("21")
#define ANSI_RESET_WEIGHT ANSI_SGR("22")
#define ANSI_RESET_ITALIC ANSI_SGR("23")
#define ANSI_RESET_UNDERLINE ANSI_SGR("24")
#define ANSI_RESET_BLINK ANSI_SGR("25")
#define ANSI_RESET_REVERSE ANSI_SGR("27")
#define ANSI_REVEAL ANSI_SGR("28")
#define ANSI_RESET_STRIKETHROUGH ANSI_SGR("29")

#define ANSI_FG_BLACK ANSI_SGR("30")
#define ANSI_FG_RED ANSI_SGR("31")
#define ANSI_FG_GREEN ANSI_SGR("32")
#define ANSI_FG_YELLOW ANSI_SGR("33")
#define ANSI_FG_BLUE ANSI_SGR("34")
#define ANSI_FG_MAGENTA ANSI_SGR("35")
#define ANSI_FG_CYAN ANSI_SGR("36")
#define ANSI_FG_WHITE ANSI_SGR("37")
#define ANSI_FG_8_BIT(n) ANSI_SGR("38;5;" n "m")
#define ANSI_FG_24_BIT(r, g, b) ANSI_SGR("38;2;" r ";" g ";" b "m")
#define ANSI_FG_DEFAULT ANSI_SGR("39")

#define ANSI_BG_BLACK ANSI_SGR("40")
#define ANSI_BG_RED ANSI_SGR("41")
#define ANSI_BG_GREEN ANSI_SGR("42")
#define ANSI_BG_YELLOW ANSI_SGR("43")
#define ANSI_BG_BLUE ANSI_SGR("44")
#define ANSI_BG_MAGENTA ANSI_SGR("45")
#define ANSI_BG_CYAN ANSI_SGR("46")
#define ANSI_BG_WHITE ANSI_SGR("47")
#define ANSI_BG_8_BIT(n) ANSI_SGR("48;5;" n "m")
#define ANSI_BG_24_BIT(r, g, b) ANSI_SGR("48;2;" r ";" g ";" b "m")
#define ANSI_BG_DEFAULT ANSI_SGR("49")

#define ANSI_FG_BRIGHT_BLACK ANSI_SGR("90")
#define ANSI_FG_BRIGHT_RED ANSI_SGR("91")
#define ANSI_FG_BRIGHT_GREEN ANSI_SGR("92")
#define ANSI_FG_BRIGHT_YELLOW ANSI_SGR("93")
#define ANSI_FG_BRIGHT_BLUE ANSI_SGR("94")
#define ANSI_FG_BRIGHT_MAGENTA ANSI_SGR("95")
#define ANSI_FG_BRIGHT_CYAN ANSI_SGR("96")
#define ANSI_FG_BRIGHT_WHITE ANSI_SGR("97")

#define ANSI_BG_BRIGHT_BLACK ANSI_SGR("100")
#define ANSI_BG_BRIGHT_RED ANSI_SGR("101")
#define ANSI_BG_BRIGHT_GREEN ANSI_SGR("102")
#define ANSI_BG_BRIGHT_YELLOW ANSI_SGR("103")
#define ANSI_BG_BRIGHT_BLUE ANSI_SGR("104")
#define ANSI_BG_BRIGHT_MAGENTA ANSI_SGR("105")
#define ANSI_BG_BRIGHT_CYAN ANSI_SGR("106")
#define ANSI_BG_BRIGHT_WHITE ANSI_SGR("107")

// clang-format off
#define FORMAT_SPECIFIER(x)           \
    _Generic((x),                       \
        /* char */                    \
        char: "%c",                   \
                                      \
        /* signed integral types */   \
        signed char: "%d"             \
        short: "%d"                   \
        int: "%d"                     \
        long: "%d"                    \
        long long: "%d",              \
        __int128: "%d",               \
                                      \
        /* unsigned integral types */ \
        bool: "%u",                   \
        unsigned char: "%u",          \
        unsigned short: "%u",         \
        unsigned int: "%u",           \
        unsigned long: "%u",          \
        unsigned long long: "%u",     \
        __uint128: "%u",              \
                                      \
        /* floating point types */    \
        float: "%f",                  \
        double: "%f",                 \
        long double: "%f",            \
        _Decimal32: "%f"              \
        _Decimal64: "%f",             \
        _Decimal128: "%f"             \
        float _Complex: "%f",         \
        double _Complex: "%f",        \
        long double _Complex: "%f",   \
        float _Imaginary: "%f",       \
        double _Imaginary: "%f",      \
        long double _Imaginary: "%f")
// clang-format on

#endif
