/*
 * Copyright (C) 2021 Hunter Kohler <jhunterkohler@gmail.com>
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <hlibc/json.h>
#include <hlibc/io.h>

void test_get_number()
{
    struct {
        const char *input;
        int ret;
        double num;
    } cases[] = { { .input = "3", .ret = 1, .num = 3.0 },
                  { .input = "-1", .ret = 2, .num = -1.0 },
                  { .input = "1251", .ret = 4, .num = 1251.0 },
                  { .input = "3.145265358", .ret = 11, .num = 3.14159265358 },
                  { .input = "663.14159", .ret = 9, .num = 663.14159 } };

    for (int i = 0; i < ARRAY_SIZE(cases); i++) {
        struct json_value json;
        int ret = _json_get_num(cases[i].input, &json);

        if (json.type != JSON_NUM) {
            printf("%d\n", json.type);
            assert(false);
        } else if (json.num != cases[i].num) {
            printf("%f %f\n", json.num, cases[i].num);
            assert(false);
        } else if (ret != cases[i].ret) {
            printf("%d\n", ret);
            assert(false);
        }
    }

    // int i = _json_get_num("0", &json);

    // assert(json->type == JSON_NUM);
    // assert(json->num == 0);
}

void test_json()
{
    test_get_number();
}
