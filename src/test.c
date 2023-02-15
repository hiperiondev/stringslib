/**
 * @file test.c
 * @brief strings library test
 * @copyright 2023 Emiliano Augusto Gonzalez (hiperiondev). This project is released under MIT license. Contact: egonzalez.hiperion@gmail.com
 * @see Project Site: https://github.com/hiperiondev/stringslib
 * @note This is based on https://github.com/alcover/buf and others. Please contact their authors for more information.
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "strings_buf.h"
#include "strings_functions.h"

int main(void) {
    const char *foo = "foo";
    const char *bar = "bar";
    const char *big = "bigbigbigbigbigbigbigbig";
    const int i = 1;
    const size_t cap = 10;
    String buf, a, b, c, cpy;
    char cat[100];
    int rc;
    uint32_t res;
    string_hash_t *hash;

#define string_test_end(str) (str->data[str->len + 1] != '\0') ? 0 : 1;
#define check(buf, cap, data)                                 \
            do {                                              \
                assert (string_buf_cap(buf) == cap);          \
                assert (string_len(buf) == strlen(data));     \
                assert (!strcmp(string_buf_data(buf), data)); \
            } while(0)

    // init
    buf = string_buf_new(cap);
    check(buf, cap, "");

    // basic append
    rc = string_buf_append(buf, foo);
    assert(rc == strlen(foo));
    check(buf, cap, foo);

    rc = string_buf_append(buf, bar);
    assert(rc == strlen(bar));
    sprintf(cat, "%s%s", foo, bar);
    check(buf, cap, cat);

    // reset
    string_buf_reset(buf);
    check(buf, cap, "");
    free(buf);

    // format append
    buf = string_buf_new(cap);
    rc = string_buf_append(buf, "%s%s%d", foo, bar, i);
    sprintf(cat, "%s%s%d", foo, bar, i);
    assert(rc == strlen(cat));
    check(buf, cap, cat);
    free(buf);

    // append too large
    buf = string_buf_new(strlen(big) - 1);
    rc = string_buf_append(buf, big);
    assert(rc == 0);
    assert(string_len(buf) == 0);
    free(buf);

    // write
    buf = string_buf_new(cap);
    rc = string_buf_write(buf, foo);
    assert(rc == strlen(foo));
    check(buf, cap, foo);

    // format write
    rc = string_buf_write(buf, "%s%s%d", foo, bar, i);
    sprintf(cat, "%s%s%d", foo, bar, i);
    assert(rc == strlen(cat));
    check(buf, cap, cat);
    free(buf);

    // write too large
    assert(strlen(foo) + strlen(big) > cap);
    buf = string_buf_new(cap);
    string_buf_write(buf, foo);
    string_buf_write(buf, big);
    check(buf, cap, foo);
    free(buf);

    // dup
    buf = string_buf_new(cap);
    string_buf_append(buf, foo);
    cpy = string_buf_dup(buf);
    check(cpy, cap, foo);
    free(buf);
    free(cpy);

    // resize
    const size_t needsz = strlen(foo) + strlen(big);
    assert(needsz > cap);
    buf = string_buf_new(cap);
    string_buf_append(buf, foo);
    string_buf_resize(&buf, needsz);
    string_buf_append(buf, big);
    sprintf(cat, "%s%s", foo, big);
    check(buf, needsz, cat);
    free(buf);

    // equal
    a = string_buf_new(cap);
    b = string_buf_new(cap);
    string_buf_append(a, foo);
    string_buf_append(b, foo);
    assert(string_buf_equal(a, b));
    free(a);
    free(b);

    printf("string_buf tests OK\n");

    a = string_buf_init("es un test");
    buf = string_left(a, 4);
    assert(string_buf_equal_const(buf, "es un"));
    free(a);
    free(buf);

    a = string_buf_init("es un test");
    buf = string_right(a, 6);
    assert(string_buf_equal_const(buf, "test"));
    free(a);
    free(buf);

    a = string_buf_init("es un test");
    buf = string_mid(a, 4, 5);
    assert(string_buf_equal_const(buf, "un"));
    free(a);
    free(buf);

    a = string_buf_init("es un test");
    b = string_buf_init(" y mas cosas");
    buf = string_concat(a, b);
    assert(string_buf_equal_const(buf, "es un test y mas cosas"));
    free(a);
    free(b);
    free(buf);

    a = string_buf_init("es un test");
    b = string_buf_init(" hermoso");
    buf = string_insert(a, b, 5);
    assert(string_buf_equal_const(buf, "es un hermoso test"));
    free(a);
    free(b);
    free(buf);

    a = string_buf_init("es un test");
    buf = string_delete(a, 3, 5);
    assert(string_buf_equal_const(buf, "es test"));
    free(a);
    free(buf);

    a = string_buf_init("es un test");
    b = string_buf_init("un");
    res = string_find(a, b);
    assert(res == 3);
    free(a);
    free(b);

    a = string_buf_init("es un test");
    b = string_buf_init("un");
    c = string_buf_init("otro");
    buf = string_replace(a, b, c);
    assert(string_buf_equal_const(buf, "es otro test"));
    free(a);
    free(b);
    free(c);
    free(buf);

    a = string_buf_init("es Un test");
    buf = string_toupper(a);
    assert(string_buf_equal_const(buf, "ES UN TEST"));
    free(a);
    free(buf);

    a = string_buf_init("ES un TEST");
    buf = string_tolower(a);
    assert(string_buf_equal_const(buf, "es un test"));
    free(a);
    free(buf);

    a = string_buf_init("es un@test");
    uint32_t r = string_find_c(a, '@', 0);
    assert(r == 5);
    free(a);

    a = string_buf_init("   es un test   ");
        buf = string_trim(a);
        assert(string_buf_equal_const(buf, "es un test"));
        free(a);
        free(buf);

    uint8_t key[16] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF };
    a = string_buf_init("Esto es un Test para hash");
    b = string_buf_new(32);
    hash = string_hash(a, SIP128, key);
    for (int n = 0; n < hash->outlen; n++)
        string_buf_append(b, "%02x", hash->out[n]);
    assert(string_buf_equal_const(b, "1882ec9b9f416a6330aecc8b1bfafd13"));
    free(a);
    free(b);
    free(hash);

    a = string_buf_init("Esto es un Test para hash");
    b = string_buf_new(32);
    hash = string_hash(a, HSIP64, key);
    for (int n = 0; n < hash->outlen; n++)
        string_buf_append(b, "%02x", hash->out[n]);
    assert(string_buf_equal_const(b, "eac1d8508e6a7f5a"));
    free(a);
    free(b);
    free(hash);

    printf("string_functions tests OK\n");

#undef check
#undef string_test_end

    return EXIT_SUCCESS;
}
