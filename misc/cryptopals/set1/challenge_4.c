#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

int main()
{
    // FILE *fp = fopen("./cryptopals/set_1/challenge_4_data.txt", "r");

    // double min_err = INFINITY;
    // byte_t min_text[100] = { 0 };

    // ssize_t len;
    // size_t linecap;
    // byte_t *line;

    // while ((len = getline((char **)&line, &linecap, fp)) > 0) {
    //     double err = 0;
    //     byte_t text[100];

    //     hex_decode(line, 60, text);
    //     byte_t key = single_xor_key(text, 30);
    //     single_xor_apply(text, 30, key);

    //     err = en_frequency_error(text, 30);

    //     if (err < min_err) {
    //         min_err = err;
    //         memcpy(min_text, text, 30);
    //     }
    // }

    // free(line);
    // fclose(fp);

    // assert(!memcmp("Now that the party is jumping", min_text, 29));
    // printf("Success\n");
}
