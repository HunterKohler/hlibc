/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef RELJUMP_H_
#define RELJUMP_H_

#include <stdint.h>

struct reljump {
    uint8_t opcode;
    int32_t len;
} __attribute__((packed));

int replace_func(void *func, void *repl);

#endif
