# Project Style

Project level styling guidelines.

## Macros

Macros should primarily be another method of hiding implementation details, and specifying compiler-constants, or static analysis notation.

Do not redefine common macros like `array_size()`. Use that provided in `<hlibc/def.h>`.

Function-like macros may use function naming rules if they are relatively 'normal' (no input re-evaluation, etc.). Re-evaluation should be specifically noted if it occurs.

Macros for the purposes of language features should be avoided at all cost. The code becomes unreadable and uninterpretable by those unfamiliar with your new language.

## Enums

Do not typedef enums. Enum type names should be snake-case like other identifiers. Enum values should, of course, be constant-case. Even though an enum may not be used a flag, it is often good to consider making their values bitwise independent for the purposes of constant identification.

## Structures

Use snake case for structure names. Do not use `_s`, `s_` as a postfix or prefix, that's what the keyword `struct` is for.

## Typdefs

Use typedefs for opaque types; use typedefs for convenience when it is actually really convenient; like: `thread_t` instead of `struct some_other_thread_library_and_type`. This practice should be followed in source as well as headers.

Do not use non-standard width types like `u8`, `u16`, etc. Alternatively use `<stdint.h>`. That being said, avoid fixed width integers unless it the width is relevant to the code.

Use the typedefs `int128_t` , `uint128_t` in `<hlibc/def.h>` instead of the raw `__int128` and `unsigned __int128` types. The purpose, like with most typedefs, is to give control over the definition, so, for example, when we try to compile on a non-supporting machine/compiler, the typedef can be removed or changed.

## Crypto

Input/output to crypto algorithms should be considered little endian unless specifically specified otherwise. To maintain consistent API's, and reduce mental strain, take input by pointer, and write to pointer, instead of returning an integer. This should even be applied to utility hashes. Endian mistakes frequently occur when directly returning a value instead of writing, and non full int-width hashes are difficult to deal with, or impossible, since no one supports large types like 512 bits.
