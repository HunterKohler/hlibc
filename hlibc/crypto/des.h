/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#ifndef HLIBC_CRYPTO_DES_H_
#define HLIBC_CRYPTO_DES_H_

#include <hlibc/def.h>

uint32_t des_f(uint32_t block, uint64_t key);
uint64_t des_IP(uint64_t input);
uint64_t des_IP_inv(uint64_t input);
uint64_t des_E(uint32_t input);
uint32_t des_P(uint32_t input);
uint32_t des_S(uint64_t input);
uint64_t des_left_shift(uint64_t key, size_t n);
uint64_t des_PC_1(uint64_t key);
uint64_t des_PC_2(uint64_t key);
uint64_t des_single(uint64_t block, uint64_t key);
void des_schedule(uint64_t key, uint64_t *schedule);
void des(void *input, size_t n, uint64_t key);

#endif
