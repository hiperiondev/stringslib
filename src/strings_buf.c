/**
 * @file strings_buf.c
 * @brief
 * @copyright 2022 Emiliano Augusto Gonzalez (hiperiondev). This project is released under MIT license. Contact: egonzalez.hiperion@gmail.com
 * @see Project Site: https://github.com/hiperiondev/stringslib
 * @note This is based on https://github.com/alcover/buf. Please contact their authors for more information.
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "strings_buf.h"

// mem size of a Buffer with capacity cap
// header + data + sentinel 
#define BUF_CHR       (sizeof ((string_t){0}).data[0])
#define BUF_MEM(cap)  (sizeof(string_t) + (cap + 1) * BUF_CHR)

/*
 Allocate a new Buffer of capacity `cap`.
 Returns: Buf|NULL
 */
String string_buf_new(const size_t cap) {
    String buf = malloc(BUF_MEM(cap));

    if (buf) {
        buf->cap = cap;
        buf->len = 0;
        buf->data[0] = 0;
        buf->data[cap] = 0;
    }
    
    return buf;
}

/*
 Allocate a new Buffer of capacity `cap` and copy string
 Returns: Buf|NULL
 */
String string_buf_init(const char *str) {
    if (str == NULL)
        return NULL;

    String buf = string_buf_new(strlen(str));
    memcpy(buf->data, str, strlen(str));
    buf->len = strlen(str);

    return buf;
}

/*
 Append a formatted c-string to `buf`.
 If new data would exceed capacity, `buf` stays unmodified.
 Returns: change in length.
 Ex: buf_append (buf, str);
 buf_append (buf, "Hello");
 buf_append (buf, "%s has %d apples", "Mary", 10);
 */
int string_buf_append(String buf, const char *fmt, ...) {
    if (!fmt)
        return 0;

    const size_t spc = buf->cap - buf->len;
    
    if (!spc)
        return 0;

    // get potential write length
    va_list args;
    va_start(args, fmt);
    const int len = vsnprintf(NULL, 0, fmt, args); //rem: end null not counted
    va_end(args);

    if (len > spc)
        return 0;

    char *end = buf->data + buf->len;

    errno = 0;
    va_start(args, fmt);
    int written = vsnprintf(end, spc + 1, fmt, args);
    va_end(args);

    if (written < 0) {
        perror("buf_append");
        *end = 0; // just in case..
        return 0;
    }

    // truncated - useless?
    if (written > spc) {
        *end = 0;
        return 0;
    }

    buf->len += written;

    return written;
}

/*
 Write a formatted c-string at beginning of `buf`.
 If new data would exceed capacity, `buf` stays unmodified.
 Returns: new length or zero on failure.
 */
int string_buf_write(String buf, const char *fmt, ...) {
    if (!fmt)
        return 0;

    const size_t cap = buf->cap;

    if (!cap)
        return 0;

    // get potential write length
    va_list args;
    va_start(args, fmt);
    const int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if (len > cap)
        return 0;

    errno = 0;
    va_start(args, fmt);
    const int written = vsnprintf(buf->data, cap + 1, fmt, args);
    va_end(args);
    
    if (written < 0) {
        perror("buf_write");
        return 0;
    }

    buf->len = written;

    return written;
}

bool string_buf_equal(const String a, const String b) {
    if (!a && !b)
        return true; //?
    if (!a || !b)
        return false; //?

    const size_t lena = a->len;
    const size_t lenb = b->len;

    if (lena != lenb)
        return false;

    return !memcmp(a->data, b->data, lena);
}

bool string_buf_equal_const(const String a, const char *b) {
    if (a == NULL || b == NULL)
        return false;

    const size_t lena = a->len;
    const size_t lenb = strlen(b);

    if (lena != lenb)
        return false;

    return !memcmp(a->data, b, lena);
}

// todo: faster w/o calling new()
String string_buf_dup(const String buf) {
    String ret = string_buf_new(buf->cap);

    if (ret) {
        // copies only up to current length
        memcpy(ret, buf, BUF_MEM(buf->len));
    }

    return ret;
}

bool string_buf_resize(String *pbuf, const size_t newcap) {
    String buf = *pbuf;
    
    if (newcap == buf->cap)
        return true;

    uint32_t buflen = buf->len;

    String tmp = realloc(buf, BUF_MEM(newcap));
    
    if (!tmp) {
        fprintf(stderr, "buf_resize failed\n");
        return false;
    }
    
    // truncated
    if (newcap < buflen) {
        tmp->data[newcap] = 0;
        tmp->len = newcap;
    }

    tmp->cap = newcap;
    *pbuf = tmp;
    
    return true;
}

/* Accessories */
size_t string_buf_cap(const String buf) {
    return buf->cap;
}

const char* string_buf_data(const String buf) {
    return buf->data;
}

void string_buf_reset(String buf) {
    buf->len = 0;
    buf->data[0] = 0;
}
