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

#include "strings.h"

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
    bool bres;
    double dres;
    long lres;
    string_hash_t hash;
    String *array;

#define string_test_end(str) (str->data[str->len + 1] != '\0') ? 0 : 1;
#define check(buf, cap, data)                             \
            do {                                          \
                assert (buf->capacity == cap);            \
                assert (buf->length == strlen(data));     \
                assert (!strcmp(string_data(buf), data)); \
            } while(0)

    buf = string_new(cap);
    check(buf, cap, "");

    rc = string_append(buf, foo);
    assert(rc == strlen(foo));
    check(buf, cap, foo);

    rc = string_append(buf, bar);
    assert(rc == strlen(bar));
    sprintf(cat, "%s%s", foo, bar);
    check(buf, cap, cat);

    string_reset(buf);
    check(buf, cap, "");
    free(buf);

    buf = string_new(cap);
    rc = string_append(buf, "%s%s%d", foo, bar, i);
    sprintf(cat, "%s%s%d", foo, bar, i);
    assert(rc == strlen(cat));
    check(buf, cap, cat);
    free(buf);

    buf = string_new(strlen(big) - 1);
    rc = string_append(buf, big);
    assert(rc == 0);
    assert(buf->length == 0);
    free(buf);

    buf = string_new(cap);
    rc = string_write(buf, foo);
    assert(rc == strlen(foo));
    check(buf, cap, foo);

    rc = string_write(buf, "%s%s%d", foo, bar, i);
    sprintf(cat, "%s%s%d", foo, bar, i);
    assert(rc == strlen(cat));
    check(buf, cap, cat);
    free(buf);

    assert(strlen(foo) + strlen(big) > cap);
    buf = string_new(cap);
    string_write(buf, foo);
    string_write(buf, big);
    check(buf, cap, foo);
    free(buf);

    buf = string_new(cap);
    string_append(buf, foo);
    cpy = string_dup(buf);
    check(cpy, cap, foo);
    free(buf);
    free(cpy);

    const size_t needsz = strlen(foo) + strlen(big);
    assert(needsz > cap);
    buf = string_new(cap);
    string_append(buf, foo);
    string_resize(&buf, needsz);
    string_append(buf, big);
    sprintf(cat, "%s%s", foo, big);
    check(buf, needsz, cat);
    free(buf);

    a = string_new_c("es un test");
    b = string_new_c(" y mas cosas");
    res = string_move(&a, &b);
    assert(string_equals_c(a, " y mas cosas"));
    free(a);

    a = string_new_c("es un test");
    res = string_copy(&a, "pruebita");
    assert(string_equals_c(a, "pruebita"));
    free(a);

    printf("string_core tests OK\n");

    a = string_new_c("es un test");
    buf = string_left(a, 4);
    assert(string_equals_c(buf, "es un"));
    free(a);
    free(buf);

    a = string_new_c("es un test");
    buf = string_right(a, 6);
    assert(string_equals_c(buf, "test"));
    free(a);
    free(buf);

    a = string_new_c("es un test");
    buf = string_mid(a, 4, 5);
    assert(string_equals_c(buf, "un"));
    free(a);
    free(buf);

    a = string_new_c("es un test");
    b = string_new_c(" y mas cosas");
    buf = string_concat(a, b);
    assert(string_equals_c(buf, "es un test y mas cosas"));
    free(a);
    free(b);
    free(buf);

    a = string_new_c("es un test");
    b = string_new_c(" hermoso");
    buf = string_insert(a, b, 5);
    assert(string_equals_c(buf, "es un hermoso test"));
    free(a);
    free(b);
    free(buf);

    a = string_new_c("es un test");
    buf = string_delete(a, 3, 5);
    assert(string_equals_c(buf, "es test"));
    free(a);
    free(buf);

    a = string_new_c("es un test");
    buf = string_delete_c(a, "un ");
    assert(string_equals_c(buf, "es test"));
    free(a);
    free(buf);

    a = string_new_c("es un test");
    buf = string_delete_prefix_c(a, "es  ");
    assert(string_equals_c(buf, "un test"));
    free(a);
    free(buf);

    a = string_new_c("es un test");
    buf = string_delete_postfix_c(a, " test");
    assert(string_equals_c(buf, "es un"));
    free(a);
    free(buf);

    a = string_new_c("es un test");
    b = string_new_c("un");
    res = string_find(a, b, 0);
    assert(res == 3);
    res = string_find(a, b, 2);
    assert(res == 3);
    free(a);
    free(b);

    a = string_new_c("es un test");
    b = string_new_c("un");
    c = string_new_c("otro");
    buf = string_replace(a, b, c, 2);
    assert(string_equals_c(buf, "es otro test"));
    free(a);
    free(b);
    free(c);
    free(buf);

    a = string_new_c("es un test");
    const char *str1 = "un";
    const char *str2 = "otro";
    buf = string_replace_c(a, str1, str2, 2);
    assert(string_equals_c(buf, "es otro test"));
    free(a);
    free(buf);

    a = string_new_c("es un test");
    const char *str3 = "un ";
    const char *str4 = "";
    buf = string_replace_c(a, str3, str4, 2);
    assert(string_equals_c(buf, "es test"));
    free(a);
    free(buf);

    a = string_new_c("es un test");
    const char *str5 = "un ";
    const char *str6 = "";
    string_replace_c_m(a, str5, str6, 2);
    assert(string_equals_c(a, "es test"));
    free(a);

    a = string_new_c("es Un test");
    buf = string_toupper(a);
    assert(string_equals_c(buf, "ES UN TEST"));
    free(a);
    free(buf);

    a = string_new_c("ES un TEST");
    buf = string_tolower(a);
    assert(string_equals_c(buf, "es un test"));
    free(a);
    free(buf);

    a = string_new_c("es un@test");
    uint32_t r = string_find_c(a, "@", 0);
    assert(r == 5);
    free(a);

    a = string_new_c("   es un test   ");
    buf = string_ltrim(a);
    assert(string_equals_c(buf, "es un test   "));
    free(a);
    free(buf);

    a = string_new_c("   es un test   ");
    buf = string_rtrim(a);
    assert(string_equals_c(buf, "   es un test"));
    free(a);
    free(buf);

    a = string_new_c("   es un test   ");
    buf = string_trim(a);
    assert(string_equals_c(buf, "es un test"));
    free(a);
    free(buf);

    a = string_new_c("es un test");
    b = string_new_c("es un test");
    bres = string_equals(a, b);
    assert(bres);
    string_write(b, "otracosa");
    bres = string_equals(a, b);
    assert(!bres);
    free(a);
    free(b);

    a = string_new_c("-124");
    b = string_new_c("23.89");
    bres = string_isinteger(a);
    assert(bres);
    bres = string_isinteger(b);
    assert(!bres);
    bres = string_isfloat(b);
    assert(bres);
    bres = string_issigned(a);
    assert(bres);
    bres = string_issigned(b);
    assert(!bres);
    free(a);
    free(b);

    a = string_new_c("-23.89");
    dres = string_todouble(a);
    assert(dres == -23.89);
    free(a);

    a = string_new_c("-234567");
    lres = string_tolong(a, 10);
    assert(lres == -234567);
    free(a);

    a = string_new_c("-23.89e5");
    dres = string_todouble(a);
    assert(dres == -2389000);
    free(a);

    a = string_new_c("String de-Prueba");
    buf = string_split(a, "-", &b);
    assert(string_equals_c(buf, "String de"));
    assert(string_equals_c(b, "Prueba"));
    free(a);
    free(b);
    free(buf);

    a = string_new_c("String de Prueba para split_c");
    res = string_split_array(a, " ", &array);
    assert(res == 5);
    assert(string_equals_c(array[0], "String"));
    assert(string_equals_c(array[1], "de"));
    assert(string_equals_c(array[2], "Prueba"));
    assert(string_equals_c(array[3], "para"));
    assert(string_equals_c(array[4], "split_c"));
    for (uint32_t n = 0; n < res; n++) {
        free(array[n]);
    }
    free(a);
    free(array);

    a = string_new_c("String@T0de@T0Prueba@T0para@T0split_c");
    res = string_split_array(a, "@T0", &array);
    assert(res == 5);
    assert(string_equals_c(array[0], "String"));
    assert(string_equals_c(array[1], "de"));
    assert(string_equals_c(array[2], "Prueba"));
    assert(string_equals_c(array[3], "para"));
    assert(string_equals_c(array[4], "split_c"));
    for (uint32_t n = 0; n < res; n++) {
        free(array[n]);
    }
    free(a);
    free(array);

    a = string_new_c("String de-Prueba");
    string_splitr_m(a, "-", b);
    assert(string_equals_c(b, "String de"));
    assert(string_equals_c(a, "Prueba"));
    free(a);
    free(b);

    a = string_new_c("String de-Prueba");
    b = string_new_c("");
    c = string_new_c("       ");
    bres = string_isblank(a);
    assert(bres == 0);
    bres = string_isblank(b);
    assert(bres == 1);
    bres = string_isblank(c);
    assert(bres == 1);
    free(a);
    free(b);
    free(c);

    a = string_new_c("StringdePrueba123");
    b = string_new_c("Stringde@Prueba123");
    c = string_new_c("String_de_Prueba_123");
    bres = string_isalnum(a, 0, false);
    assert(bres == 1);
    bres = string_isalnum(b, 0, false);
    assert(bres == 0);
    bres = string_isalnum(b, 9, true);
    assert(bres == 1);
    bres = string_isalnum(c, 0, false);
    assert(bres == 0);
    bres = string_isalnum(c, 0, true);
    assert(bres == 1);
    free(a);
    free(b);
    free(c);

    uint8_t key[16] = { 0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF };
    a = string_new_c("Esto es un Test para hash");
    b = string_new(32);
    hash = string_hash(a, SIP128, key);
    for (int n = 0; n < hash.outlen; n++)
        string_append(b, "%02x", hash.out[n]);
    assert(string_equals_c(b, "1882ec9b9f416a6330aecc8b1bfafd13"));
    free(a);
    free(b);

    a = string_new_c("Esto es un Test para hash");
    b = string_new(32);
    hash = string_hash(a, HSIP64, key);
    for (int n = 0; n < hash.outlen; n++)
        string_append(b, "%02x", hash.out[n]);
    assert(string_equals_c(b, "eac1d8508e6a7f5a"));
    free(a);
    free(b);

    printf("string_functions tests OK\n");

#undef check
#undef string_test_end

    return EXIT_SUCCESS;
}
