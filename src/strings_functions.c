/**
 * @file strings_functions.c
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

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#include "strings_buf.h"
#include "strings_functions.h"
#include "siphash.h"
#include "halfsiphash.h"

/**
 * @fn size_t string_len(const String buf)
 * @brief Buffered string length
 *
 * @param buf Buffered string
 * @return Size
 */
size_t string_len(const String buf) {
    if (buf == NULL)
        return 0;

    return buf->len;
}

/**
 * @fn String string_left(const String buf, uint32_t pos)
 * @brief Substring left from position
 *
 * @param buf Buffered string
 * @param pos Position
 * @return Buffered string
 */
String string_left(const String buf, uint32_t pos) {
    if (buf == NULL || pos > buf->len)
        return NULL;

    String new = string_buf_new(pos);
    memcpy(new->data, buf->data, pos + 1);
    new->len = pos + 1;

    return new;
}

/**
 * @fn String string_right(const String buf, uint32_t pos)
 * @brief Substring right from position
 *
 * @param buf Buffered string
 * @param pos Position
 * @return Buffered string
 */
String string_right(const String buf, uint32_t pos) {
    if (buf == NULL || pos > buf->len)
        return NULL;

    String new = string_buf_new(buf->len - pos);
    memcpy(new->data, buf->data + pos, buf->len - pos + 1);
    new->len = buf->len - pos;

    return new;
}

/**
 * @fn String string_mid(const String buf, uint32_t left, uint32_t right)
 * @brief Substring left from position left to position right
 *
 * @param buf Buffered string
 * @param left Position
 * @param right Position
 * @return Buffered string
 */
String string_mid(const String buf, uint32_t left, uint32_t right) {
    if (buf == NULL || right > buf->len || left > buf->len || left > right)
        return NULL;

    String new = string_buf_new(right - left);
    memcpy(new->data, buf->data + left - 1, right - left + 1);
    new->len = right - left + 1;

    return new;
}

/**
 * @fn String string_concat(const String str1, const String str2)
 * @brief Concatenation of strings
 *
 * @param str1 Buffered string
 * @param str2 Buffered string
 * @return Buffered string
 */
String string_concat(const String str1, const String str2) {
    if (str1 == NULL || str2 == NULL)
        return NULL;

    String new = string_buf_new(str1->len + str2->len);
    memcpy(new->data, str1->data, str1->len);
    memcpy(new->data + str1->len, str2->data, str2->len + 1);
    new->len = str1->len + str2->len;

    return new;
}

/**
 * @fn String string_insert(const String buf, const String str, uint32_t pos)
 * @brief Insert string on position
 *
 * @param buf Buffered string
 * @param str Buffered string
 * @param pos Position
 * @return Buffered string
 */
String string_insert(const String buf, const String str, uint32_t pos) {
    if (buf == NULL || str == NULL || pos > buf->len)
        return NULL;

    String new = string_buf_new(buf->len + str->len);
    memcpy(new->data, buf->data, pos);
    memcpy(new->data + pos, str->data, str->len);
    memcpy(new->data + pos + str->len, buf->data + pos, (buf->len - pos) + 1);

    new->len = buf->len + str->len;

    return new;
}

/**
 * @fn String string_delete(const String buf, uint32_t pos1, uint32_t pos2)
 * @brief Delete substring from pos1 to pos2
 *
 * @param buf Buffered string
 * @param pos1 Position
 * @param pos2 Position
 * @return Buffered string
 */
String string_delete(const String buf, uint32_t pos1, uint32_t pos2) {
    if (buf == NULL || pos1 > buf->len || pos2 > buf->len || pos1 > pos2) {
        return NULL;
    }

    String new = string_buf_new(buf->len - pos2 + pos1);
    memcpy(new->data, buf->data, pos1);
    memcpy(new->data + pos1, buf->data + pos2 + 1, buf->len - pos2);

    new->len = buf->len - pos2 + pos1 - 1;

    return new;
}

/**
 * @fn String string_replace(const String buf, const String search, String replace, uint32_t pos)
 * @brief Replace string
 *
 * @param buf Buffered string
 * @param search Buffered string
 * @param replace Buffered string
 * @param pos Start position
 * @return Buffered string
 */
String string_replace(const String buf, const String search, String replace, uint32_t pos) {
    if (buf == NULL || search == NULL || replace == NULL || pos > buf->len)
        return NULL;

    uint32_t fpos = string_find(buf, search, pos);
    if (fpos == STR_ERROR)
        return NULL;

    String new = string_buf_new(buf->len - search->len + replace->len);
    memcpy(new->data, buf->data, fpos);
    memcpy(new->data + fpos, replace->data, replace->len);
    memcpy(new->data + fpos + replace->len, buf->data + search->len + fpos, buf->len - fpos - search->len);

    new->len = buf->len - search->len + replace->len;

    return new;
}

/**
 * @fn uint32_t string_find(const String buf, const String search, uint32_t pos)
 * @brief Find substring.
 *
 * @param buf Buffered string
 * @param search Buffered string
 * @param pos Start position
 * @return Position
 */
uint32_t string_find(const String buf, const String search, uint32_t pos) {
    if (buf == NULL || search == NULL || search->len > buf->len || pos > buf->len)
        return STR_ERROR;

    char *p;
    if ((p = strstr(buf->data + pos, search->data)) != NULL)
        return (p - buf->data);

    return STR_ERROR;
}

/**
 * @fn string_find_c(const String buf, char c, uint32_t pos)
 * @brief Find character starting at position.
 *
 * @param buf Buffered string
 * @param c Searched char
 * @param pos Start position
 * @return Position
 */
uint32_t string_find_c(const String buf, char c, uint32_t pos) {
    if (buf == NULL || pos > buf->len)
        return false;
    for (int p = pos; p < buf->len; p++) {
        if (buf->data[p] == c)
            return p;
    }

    return STR_ERROR;
}

/**
 * @fn String string_toupper(const String buf)
 * @brief To upper string
 *
 * @param buf Buffered string
 * @return Buffered string
 */
String string_toupper(const String buf) {
    if (buf == NULL)
        return NULL;

    String new = string_buf_new(buf->len);
    for (int i = 0; i < buf->len; i++) {
        if (buf->data[i] >= 97 && buf->data[i] <= 122)
            new->data[i] = buf->data[i] - 32;
        else new->data[i] = buf->data[i];
    }

    new->len = buf->len;

    return new;
}

/**
 * @fn String string_tolower(const String buf)
 * @brief To lower string
 *
 * @param buf Buffered string
 * @return Buffered string
 */
String string_tolower(const String buf) {
    if (buf == NULL)
        return NULL;

    String new = string_buf_new(buf->len);
    for (int i = 0; i < buf->len; i++) {
        if (buf->data[i] >= 65 && buf->data[i] <= 90)
            new->data[i] = buf->data[i] + 32;
        else new->data[i] = buf->data[i];
    }

    new->len = buf->len;

    return new;
}

/**
 * @fn String string_trim(const String buf)
 * @brief Trim string
 *
 * @param buf Buffered string
 * @return Buffered string
 */
String string_trim(const String buf) {
    if (buf == NULL)
        return NULL;

    uint32_t pos1 = 0, pos2 = buf->len - 1;

    while (isspace(buf->data[pos1]))
        ++pos1;
    while (isspace(buf->data[pos2]))
        --pos2;

    String new = string_buf_new(pos1 + pos2 + 1);
    memcpy(new->data, buf->data + pos1, pos2 - pos1 + 1);

    new->len = pos2 - pos1 + 1;

    return new;
}

/**
 * @fn string_equals(const String str1, const String str2)
 * @brief Compares two strings.
 *
 * @param str1
 * @param str2
 * @return Returns true if the strings are equal, and false if not.
 */
bool string_equals(const String str1, const String str2) {
    if (str1 == NULL || str2 == NULL || str1->len != str2->len)
        return false;

    return (strcmp(str1->data, str2->data) == 0);
}

////////////////////////////////////////////////////////////

/**
 * @fn bool string_isinteger(const String buf)
 * @brief Check if string is a valid integer
 *
 * @param buf Buffered string
 * @return boolean
 */
bool string_isinteger(const String buf) {
    if (buf == NULL)
        return false;

    int n = 0;

    if (buf->data[0] == '-')
        ++n;

    for (; n < buf->len; n++) {
        if (!isdigit(buf->data[n]))
            return false;
    }

    return true;
}

/**
 * @fn bool string_isfloat(const String buf)
 * @brief Check if string is a valid float
 *
 * @param buf Buffered string
 * @return Boolean
 */
bool string_isfloat(const String buf) {
    if (buf == NULL)
        return false;

    int n = 0;
    bool dot = false;

    if (buf->data[0] == '-')
        ++n;

    for (; n < buf->len; n++) {
        if (!isdigit(buf->data[n]) && !((buf->data[n] == '.') && !dot))
            return false;

        if (buf->data[n] == '.')
            dot = true;
    }

    return true;
}

////////////////////////////////////////////////////////////

/**
 * @fn string_hash_t string_hash(const String buf, uint8_t version, uint8_t key[16])
 * @brief String hash
 *
 * @param buf Buffered string
 * @param version enum STRING_HASH_VERSION
 * @param key Key
 * @return String hash result
 */
string_hash_t string_hash(const String buf, uint8_t version, uint8_t key[16]) {
    string_hash_t result;

    if (buf == NULL) {
        result.outlen = 0;
        return result;
    }

    const size_t lengths[4] = { 8, 16, 4, 8 };
    int len = lengths[version];
    result.outlen = len;

    if (version < 2)
        siphash(buf->data, buf->len, key, result.out, len);
    else halfsiphash(buf->data, buf->len, key, result.out, len);

    return result;
}
