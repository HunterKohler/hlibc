/*
 * Copyright (C) 2021-2022 John Hunter Kohler <jhunterkohler@gmail.com>
 */
#ifndef HLIBC_CRYPTO_DES_H_
#define HLIBC_CRYPTO_DES_H_

#include <hlibc/def.h>

enum des_flag {
    DES_ENCRYPT = 1 << 0,
    DES_DECRYPT = 1 << 1,
};

enum triple_des_flag {
    TRIPLE_DES_ENCRYPT = 1 << 0,
    TRIPLE_DES_DECRYPT = 1 << 1,
};

uint64_t des_IP(uint64_t input);
uint64_t des_IP_inv(uint64_t input);
uint64_t des_E(uint32_t input);
uint32_t des_P(uint32_t input);
uint32_t des_S(uint64_t input);
uint32_t des_f(uint32_t block, uint64_t key);
uint64_t des_left_shift(uint64_t key, size_t n);
uint64_t des_PC_1(uint64_t key);
uint64_t des_PC_2(uint64_t key);
uint64_t des(uint64_t block, uint64_t key, int flags);
uint64_t triple_des(uint64_t block, uint64_t *keys, int flags);
uint64_t __des(uint64_t block, uint64_t *schedule, int flags);
uint64_t __triple_des(uint64_t block, uint64_t **schedule, int *flags);
void des_schedule(uint64_t key, uint64_t *schedule);

#endif
