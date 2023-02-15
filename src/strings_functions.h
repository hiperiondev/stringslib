/**
 * @file strings_functions.h
 * @brief strings manipulation functions
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

#ifndef STRINGS_FUNCTIONS_H_
#define STRINGS_FUNCTIONS_H_

#include <stdint.h>

#include "strings_buf.h"

/**
 * @enum STRING_ERROR
 * @brief Strings errors
 *
 */
enum STRING_ERROR {
    STR_OK,                 /**< Ok >**/
    STR_ERROR = UINT32_MAX, /**< Generic error >**/
};

/**
 * @enum STRING_HASH_VERSION
 * @brief String hash version
 *
 */
enum STRING_HASH_VERSION {
    SIP64,  /**< SIP64 >**/
    SIP128, /**< SIP128 >**/
    HSIP32, /**< HSIP32 >**/
    HSIP64  /**< HSIP64 >**/
};

/**
 * @struct string_hash_s
 * @brief String hash result
 *
 */
struct string_hash_s {
        uint8_t out[16]; /**< hash result >**/
         size_t outlen;  /**< hash result length >**/
};
typedef struct string_hash_s string_hash_t; /**< hash result type >**/

        size_t string_len(const String buf);
        String string_left(const String buf, uint32_t pos);
        String string_right(const String buf, uint32_t pos);
        String string_mid(const String buf, uint32_t left, uint32_t right);
        String string_concat(const String str1, const String str2);
        String string_insert(const String buf, const String str, uint32_t pos);
        String string_delete(const String buf, uint32_t pos1, uint32_t pos2);
        String string_replace(const String buf, const String search, String replace);
      uint32_t string_find(const String buf, const String search);
      uint32_t string_find_c(const String buf, char c, uint32_t pos);
        String string_toupper(const String buf);
        String string_tolower(const String buf);
        String string_trim(const String buf);

          bool string_isinteger(const String buf);
          bool string_isfloat(const String buf);

string_hash_t* string_hash(const String buf, uint8_t version, uint8_t key[16]);

#endif /* STRINGS_FUNCTIONS_H_ */
