/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
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

#define TERM_CURSOR_UP(n) CSI n "A"
#define TERM_CURSOR_DOWN(n) CSI n "B"
#define TERM_CURSOR_RIGHT(n) CSI n "C"
#define TERM_CURSOR_LEFT(n) CSI n "D"
#define TERM_CURSOR_NEXT_LN(n) CSI n "E"
#define TERM_CURSOR_PREV_LN(n) CSI n "F"
#define TERM_CURSOR_GOTO(n, m) CSI n ";" m "H"
#define TERM_ERASE_IN_DISPLAY(n) CSI n "J"
#define TERM_ERASE_IN_LINE(n) CSI n "K"
#define TERM_SCROLL_UP(n) CSI n "S"
#define TERM_SCROLL_DOWN(n) CSI n "T"
#define TERM_SGR(n) CSI n "m"

#define TERM_RESET TERM_SGR("0")
#define TERM_BOLD TERM_SGR("1")
#define TERM_FAINT TERM_SGR("2")
#define TERM_ITALIC TERM_SGR("3")
#define TERM_UNDERLINE TERM_SGR("4")
#define TERM_SLOW_BLINK TERM_SGR("5")
#define TERM_FAST_BLINK TERM_SGR("6")
#define TERM_REVERSE TERM_SGR("7")
#define TERM_HIDE TERM_SGR("8")
#define TERM_STRIKETHROUGH TERM_SGR("9")
#define TERM_DOUBLE_UNDERLINE TERM_SGR("21")
#define TERM_RESET_WEIGHT TERM_SGR("22")
#define TERM_RESET_ITALIC TERM_SGR("23")
#define TERM_RESET_UNDERLINE TERM_SGR("24")
#define TERM_RESET_BLINK TERM_SGR("25")
#define TERM_RESET_REVERSE TERM_SGR("27")
#define TERM_REVEAL TERM_SGR("28")
#define TERM_RESET_STRIKETHROUGH TERM_SGR("29")

#define TERM_FG_BLACK TERM_SGR("30")
#define TERM_FG_RED TERM_SGR("31")
#define TERM_FG_GREEN TERM_SGR("32")
#define TERM_FG_YELLOW TERM_SGR("33")
#define TERM_FG_BLUE TERM_SGR("34")
#define TERM_FG_MAGENTA TERM_SGR("35")
#define TERM_FG_CYAN TERM_SGR("36")
#define TERM_FG_WHITE TERM_SGR("37")
#define TERM_FG_8_BIT(n) TERM_SGR("38;5;" n "m")
#define TERM_FG_24_BIT(r, g, b) TERM_SGR("38;2;" r ";" g ";" b "m")
#define TERM_FG_DEFAULT TERM_SGR("39")

#define TERM_BG_BLACK TERM_SGR("40")
#define TERM_BG_RED TERM_SGR("41")
#define TERM_BG_GREEN TERM_SGR("42")
#define TERM_BG_YELLOW TERM_SGR("43")
#define TERM_BG_BLUE TERM_SGR("44")
#define TERM_BG_MAGENTA TERM_SGR("45")
#define TERM_BG_CYAN TERM_SGR("46")
#define TERM_BG_WHITE TERM_SGR("47")
#define TERM_BG_8_BIT(n) TERM_SGR("48;5;" n "m")
#define TERM_BG_24_BIT(r, g, b) TERM_SGR("48;2;" r ";" g ";" b "m")
#define TERM_BG_DEFAULT TERM_SGR("49")

#define TERM_FG_BRIGHT_BLACK TERM_SGR("90")
#define TERM_FG_BRIGHT_RED TERM_SGR("91")
#define TERM_FG_BRIGHT_GREEN TERM_SGR("92")
#define TERM_FG_BRIGHT_YELLOW TERM_SGR("93")
#define TERM_FG_BRIGHT_BLUE TERM_SGR("94")
#define TERM_FG_BRIGHT_MAGENTA TERM_SGR("95")
#define TERM_FG_BRIGHT_CYAN TERM_SGR("96")
#define TERM_FG_BRIGHT_WHITE TERM_SGR("97")

#define TERM_BG_BRIGHT_BLACK TERM_SGR("100")
#define TERM_BG_BRIGHT_RED TERM_SGR("101")
#define TERM_BG_BRIGHT_GREEN TERM_SGR("102")
#define TERM_BG_BRIGHT_YELLOW TERM_SGR("103")
#define TERM_BG_BRIGHT_BLUE TERM_SGR("104")
#define TERM_BG_BRIGHT_MAGENTA TERM_SGR("105")
#define TERM_BG_BRIGHT_CYAN TERM_SGR("106")
#define TERM_BG_BRIGHT_WHITE TERM_SGR("107")

#endif
