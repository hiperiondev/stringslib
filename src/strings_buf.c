/**
 * @file strings_buf.c
 * @brief strings buffer functions
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "strings_buf.h"

/**
 * @def BUF_CHR
 * @brief size of buffered string structure
 *
 */
#define BUF_CHR       (sizeof ((string_t){0}).data[0])

/**
 * @def BUF_MEM
 * @brief size of buffered string
 *
 */
#define BUF_MEM(cap)  (sizeof(string_t) + (cap + 1) * BUF_CHR)

/**
 * @fn String string_buf_new(const size_t cap)
 * @brief Allocate a new Buffer of capacity `cap`.
 *
 * @param cap Capacity
 * @return  Buffered string
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

/**
 * @fn String string_buf_init(const char *str)
 * @brief Allocate a new Buffer of capacity `cap` and copy string
 *
 * @param str String
 * @return  Buffered string|NULL
 */
String string_buf_init(const char *str) {
    if (str == NULL) return NULL;

    String buf = string_buf_new(strlen(str));
    memcpy(buf->data, str, strlen(str));
    buf->len = strlen(str);

    return buf;
}

/**
 * @fn int string_buf_append(String buf, const char *fmt, ...)
 * @brief Append a formatted c-string to `buf`.
 *        If new data would exceed capacity, `buf` stays unmodified.
 *
 * @param buf  Buffered string
 * @param fmt Format
 * @return Change in length.
 */
int string_buf_append(String buf, const char *fmt, ...) {
    if (!fmt) return 0;

    const size_t spc = buf->cap - buf->len;
    
    if (!spc) return 0;

    // get potential write length
    va_list args;
    va_start(args, fmt);
    const int len = vsnprintf(NULL, 0, fmt, args); //rem: end null not counted
    va_end(args);

    if (len > spc) return 0;

    char *end = buf->data + buf->len;

    errno = 0;
    va_start(args, fmt);
    int written = vsnprintf(end, spc + 1, fmt, args);
    va_end(args);

    if (written < 0) {
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

/**
 * @fn int string_buf_write(String buf, const char *fmt, ...)
 * @brief Write a formatted c-string at beginning of `buf`.
 *        If new data would exceed capacity, `buf` stays unmodified.
 *
 * @param buf  Buffered string
 * @param fmt Format
 * @return New length or zero on failure.
 */
int string_buf_write(String buf, const char *fmt, ...) {
    if (!fmt) return 0;

    const size_t cap = buf->cap;

    if (!cap) return 0;

    // get potential write length
    va_list args;
    va_start(args, fmt);
    const int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if (len > cap) return 0;

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

/**
 * @fn bool string_buf_equal(const String a, const String b)
 * @brief Compare strings equality
 *
 * @param a  Buffered string
 * @param b  Buffered string
 * @return Boolean
 */
bool string_buf_equal(const String a, const String b) {
    if (!a && !b) return true; //?
    if (!a || !b) return false; //?

    const size_t lena = a->len;
    const size_t lenb = b->len;

    if (lena != lenb) return false;

    return !memcmp(a->data, b->data, lena);
}

/**
 * @fn bool string_buf_equal_const(const String a, const char *b)
 * @brief Compare strings equality
 *
 * @param a Buffered string
 * @param b String
 * @return Boolean
 */
bool string_buf_equal_const(const String a, const char *b) {
    if (a == NULL || b == NULL) return false;

    const size_t lena = a->len;
    const size_t lenb = strlen(b);

    if (lena != lenb) return false;

    return !memcmp(a->data, b, lena);
}

/**
 * @fn String string_buf_dup(const String buf)
 * @brief Duplicate string
 *
 * @param buf Buffered string
 * @return Buffer
 */
String string_buf_dup(const String buf) {
    String ret = string_buf_new(buf->cap);

    if (ret) {
        // copies only up to current length
        memcpy(ret, buf, BUF_MEM(buf->len));
    }

    return ret;
}

/**
 * @fn bool string_buf_resize(String *pbuf, const size_t newcap)
 * @brief Resize capacity
 *
 * @param pbuf  Buffered string
 * @param newcap New capacity
 * @return Boolean
 */
bool string_buf_resize(String *pbuf, const size_t newcap) {
    String buf = *pbuf;
    
    if (newcap == buf->cap) return true;

    uint32_t buflen = buf->len;

    String tmp = realloc(buf, BUF_MEM(newcap));
    
    if (!tmp)
        return false;
    
    // truncated
    if (newcap < buflen) {
        tmp->data[newcap] = 0;
        tmp->len = newcap;
    }

    tmp->cap = newcap;
    *pbuf = tmp;
    
    return true;
}
/**
 * @fn size_t string_buf_cap(const String buf)
 * @brief Return capacity
 *
 * @param buf Buffered string
 * @return Capacity
 */
size_t string_buf_cap(const String buf) {
    return buf->cap;
}

/**
 * @fn const char* string_buf_data(const String buf)
 * @brief Return Data of Buffered string
 *
 * @param buf Buffered string
 * @return String
 */
const char* string_buf_data(const String buf) {
    return buf->data;
}

/**
 * @fn void string_buf_reset(String buf)
 * @brief Reset Buffered string content
 *
 * @param buf Buffered string
 */
void string_buf_reset(String buf) {
    buf->len = 0;
    buf->data[0] = 0;
}
