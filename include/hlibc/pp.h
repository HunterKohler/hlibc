/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_PP_H_
#define HLIBC_PP_H_

#include <hlibc/generated/pp.h>

#error NOT CURRENTLY INTENDED FOR USE

#define EAT(n, ...) PASTE_2(EAT_, n)(__VA_ARGS__)
#define GET_ARG(n, ...) PASTE_2(GET_ARG_, n)(__VA_ARGS__)
#define PASTE(...) PASTE_2(PASTE_, NARGS(__VA_ARGS__))(__VA_ARGS__)
#define REPEAT(n, ...) PASTE_2(REPEAT_, n)(__VA_ARGS__)
#define REPEAT_LIST(n, ...) PASTE_2(REPEAT_LIST_, n)(__VA_ARGS__)
#define REV_SEQ(n) PASTE_2(REV_SEQ_, n)()
#define SEQ(n) PASTE_2(SEQ_, n)()

/*
 * Returns `1` if expression contains comma delimiting tokens, `0` otherwise.
 */
#define HAS_COMMA(...) \
    GET_ARG(PP_MAX, __VA_ARGS__, EAT(1, REPEAT_LIST(PP_MAX, 1)), 0)

#define IF_ELSE(cond, _if, _else) PASTE_2(GET_ARG_, cond)(_else, _if)

#define NARGS(...)                                         \
    IF_ELSE(HAS_COMMA(__VA_ARGS__),                        \
            GET_ARG(PP_MAX, __VA_ARGS__, REV_SEQ(PP_MAX)), \
            IF_ELSE(IS_EMPTY(__VA_ARGS__), 0, 1))

////////////////////////////////////////////////////////////////////////////////
//  Jens Gustedt, June 8, 2010, credited for IS_EMPTY
// https://gustedt.wordpress.com/2010/06/08/detect-empty-macro-arguments/
////////////////////////////////////////////////////////////////////////////////
#define _IS_EMPTY(_0, _1, _2, _3) \
    HAS_COMMA(PASTE_5(_IS_EMPTY_CASE_, _0, _1, _2, _3))
#define _IS_EMPTY_CASE_0001 ,
#define _TRIGGER_PARENTHESIS_(...) ,

#define IS_EMPTY(...)                                       \
    _IS_EMPTY(HAS_COMMA(__VA_ARGS__),                       \
              HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__), \
              HAS_COMMA(__VA_ARGS__(/*empty*/)),            \
              HAS_COMMA(_TRIGGER_PARENTHESIS_ __VA_ARGS__(/*empty*/)))
////////////////////////////////////////////////////////////////////////////////

#endif
