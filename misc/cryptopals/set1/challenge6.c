#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>
#include <hlibc/string.h>
#include <hlibc/math.h>
#include "shared.h"

extern const char challenge_data[];
extern const char expected[];

struct keysize {
    size_t size;
    double error;
};

struct key {
    size_t size;
    uint8_t *buf;
    double error;
};

int keysize_error_cmp(const void *a, const void *b)
{
    const struct keysize *x = a;
    const struct keysize *y = b;

    return (x->error < y->error) * -1 + (x->error > y->error) * 1;
}

int key_cmp(const void *a, const void *b)
{
    const struct key *x = a;
    const struct key *y = b;

    return (x->error < y->error) * -1 + (x->error > y->error) * 1;
}

int main()
{
    const char dist_a[] = "this is a test";
    const char dist_b[] = "wokka wokka!!!";
    size_t dist_expected = 37;
    size_t dist = hamming_dist(dist_a, dist_b, strlen(dist_a));

    assert(dist == dist_expected);

    size_t decoded_size = strlen(challenge_data) / 4 * 3 - 1;
    char *decoded = malloc(decoded_size);
    assert(decoded);

    int decode_err = b64_decode(challenge_data, decoded, decoded_size);
    assert(!decode_err);

    size_t min_keysize = 1;
    size_t max_keysize = 256;
    size_t keysize_sample = 10;
    size_t blocks = 5;

    size_t ks_len = 0;
    size_t ks_cap = max_keysize - min_keysize;
    struct keysize *ks = malloc(ks_cap * sizeof(*ks));

    assert(ks);

    for (size_t i = min_keysize; i < max_keysize; i++) {
        struct keysize *k = ks + ks_len++;

        k->error = 0;
        k->size = i;

        for (int j = 1; j < blocks; j++) {
            k->error += hamming_dist(decoded, decoded + j * k->size, k->size);
        }

        k->error /= k->size;
    }

    qsort(ks, ks_len, sizeof(*ks), keysize_error_cmp);

    ks_len = keysize_sample;
    ks_cap = keysize_sample;
    ks = realloc(ks, ks_cap * sizeof(*ks));

    assert(ks);

    size_t keys_len = 0;
    struct key *keys = malloc(ks_len * sizeof(*keys));

    assert(keys);

    for (int i = 0; i < ks_len; i++) {
        struct key *k = keys + keys_len++;

        k->size = ks[i].size;
        k->buf = malloc(k->size);
        k->error = 0;

        assert(k->buf);

        for (int key_index = 0; key_index < k->size; key_index++) {
            double min_error = INFINITY;

            for (int c = 0; c < 256; c++) {
                int count[256] = { 0 };
                int ctrl = 0;
                int nprint = 0;
                double error = 0;

                for (int j = key_index; j < decoded_size; j += k->size) {
                    uint8_t res = decoded[j] ^ c;
                    count[res] += 1;
                    ctrl += iscntrl(res);
                    nprint += !isprint(res);
                }

                error += (ctrl * ctrl) * 0.01 + (nprint * nprint) * 0.01;

                for (int j = 0; j < 256; j++) {
                    error += fabs(en_letter_freq[j] -
                                  (double)count[j] * k->size / decoded_size);
                }

                if (error < min_error) {
                    min_error = error;
                    k->buf[key_index] = c;
                }
            }

            k->error += min_error;
        }
    }

    qsort(keys, keys_len, sizeof(*keys), key_cmp);

    struct key *main_key = keys;
    char *out = malloc(decoded_size + 1);
    out[decoded_size] = 0;

    for (int i = 0; i < decoded_size; i++) {
        out[i] = main_key->buf[i % main_key->size] ^ decoded[i];
    }

    assert(!strcmp(out, expected));

    for (int i = 0; i < keys_len; i++) {
        free(keys[i].buf);
    }

    free(out);
    free(ks);
    free(keys);
    free(decoded);
}

const char challenge_data[] =
    "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";

const char expected[] =
    "I'm back and I'm ringin' the bell \n"
    "A rockin' on the mike while the fly girls yell \n"
    "In ecstasy in the back of me \n"
    "Well that's my DJ Deshay cuttin' all them Z's \n"
    "Hittin' hard and the girlies goin' crazy \n"
    "Vanilla's on the mike, man I'm not lazy. \n"
    "\n"
    "I'm lettin' my drug kick in \n"
    "It controls my mouth and I begin \n"
    "To just let it flow, let my concepts go \n"
    "My posse's to the side yellin', Go Vanilla Go! \n"
    "\n"
    "Smooth 'cause that's the way I will be \n"
    "And if you don't give a damn, then \n"
    "Why you starin' at me \n"
    "So get off 'cause I control the stage \n"
    "There's no dissin' allowed \n"
    "I'm in my own phase \n"
    "The girlies sa y they love me and that is ok \n"
    "And I can dance better than any kid n' play \n"
    "\n"
    "Stage 2 -- Yea the one ya' wanna listen to \n"
    "It's off my head so let the beat play through \n"
    "So I can funk it up and make it sound good \n"
    "1-2-3 Yo -- Knock on some wood \n"
    "For good luck, I like my rhymes atrocious \n"
    "Supercalafragilisticexpialidocious \n"
    "I'm an effect and that you can bet \n"
    "I can take a fly girl and make her wet. \n"
    "\n"
    "I'm like Samson -- Samson to Delilah \n"
    "There's no denyin', You can try to hang \n"
    "But you'll keep tryin' to get my style \n"
    "Over and over, practice makes perfect \n"
    "But not if you're a loafer. \n"
    "\n"
    "You'll get nowhere, no place, no time, no girls \n"
    "Soon -- Oh my God, homebody, you probably eat \n"
    "Spaghetti with a spoon! Come on and say it! \n"
    "\n"
    "VIP. Vanilla Ice yep, yep, I'm comin' hard like a rhino \n"
    "Intoxicating so you stagger like a wino \n"
    "So punks stop trying and girl stop cryin' \n"
    "Vanilla Ice is sellin' and you people are buyin' \n"
    "'Cause why the freaks are jockin' like Crazy Glue \n"
    "Movin' and groovin' trying to sing along \n"
    "All through the ghetto groovin' this here song \n"
    "Now you're amazed by the VIP posse. \n"
    "\n"
    "Steppin' so hard like a German Nazi \n"
    "Startled by the bases hittin' ground \n"
    "There's no trippin' on mine, I'm just gettin' down \n"
    "Sparkamatic, I'm hangin' tight like a fanatic \n"
    "You trapped me once and I thought that \n"
    "You might have it \n"
    "So step down and lend me your ear \n"
    "'89 in my time! You, '90 is my year. \n"
    "\n"
    "You're weakenin' fast, YO! and I can tell it \n"
    "Your body's gettin' hot, so, so I can smell it \n"
    "So don't be mad and don't be sad \n"
    "'Cause the lyrics belong to ICE, You can call me Dad \n"
    "You're pitchin' a fit, so step back and endure \n"
    "Let the witch doctor, Ice, do the dance to cure \n"
    "So come up close and don't be square \n"
    "You wanna battle me -- Anytime, anywhere \n"
    "\n"
    "You thought that I was weak, Boy, you're dead wrong \n"
    "So come on, everybody and sing this song \n"
    "\n"
    "Say -- Play that funky music Say, go white boy, go white boy go \n"
    "play that funky music Go white boy, go white boy, go \n"
    "Lay down and boogie and play that funky music till you die. \n"
    "\n"
    "Play that funky music Come on, Come on, let me hear \n"
    "Play that funky music white boy you say it, say it \n"
    "Play that funky music A little louder now \n"
    "Play that funky music, white boy Come on, Come on, Come on \n"
    "Play that funky music \n";
