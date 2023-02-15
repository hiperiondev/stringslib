/**
 * @file strings_buf.h
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

#ifndef STRINGS_BUF_H
#define STRINGS_BUF_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @struct string_s
 * @brief Buffered string structure
 *
 */
typedef struct string_s {
    uint32_t cap;         /**< capacity >**/
    uint32_t len;         /**< current length >**/
        char data[];      /**< null-terminated string >**/
} string_t;               /**< Buffered string internal type >**/
typedef string_t *String; /**< Buffered string main type >**/

     String string_buf_new(const size_t cap);
     String string_buf_init(const char *str);
        int string_buf_append(String buf, const char *fmt, ...);
        int string_buf_write(String buf, const char *fmt, ...);
       bool string_buf_equal_const(const String a, const char *b);
     String string_buf_dup(const String buf);
       bool string_buf_resize(String *pbuf, const size_t newcap);
       void string_buf_reset(String buf);
     size_t string_buf_cap(const String buf);
const char* string_buf_data(const String buf);

#endif /* STRINGS_BUF_H */
