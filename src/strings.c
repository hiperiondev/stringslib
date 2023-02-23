/**
 * @file strings.c
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
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

#include "strings.h"
#include "siphash.h"
#include "halfsiphash.h"

///// core /////

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
String string_new(const size_t cap) {
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
 * @fn String string_buf_new_c(const char *str)
 * @brief Allocate a new Buffer of capacity `cap` and copy string
 *
 * @param str String
 * @return  Buffered string|NULL
 */
String string_new_c(const char *str) {
    if (str == NULL || strlen(str) > UINT32_MAX - 1)
        return NULL;

    String buf = string_new(strlen(str));
    memcpy(buf->data, str, strlen(str));
    buf->len = strlen(str);

    return buf;
}

/**
 * @fn String string_buf_dup(const String buf)
 * @brief Duplicate string
 *
 * @param buf Buffered string
 * @return Buffer
 */
String string_dup(const String buf) {
    if (buf == NULL)
        return NULL;

    String ret = string_new(buf->cap);

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
 * @param pbuf Buffered string
 * @param newcap New capacity
 * @return Boolean
 */
bool string_resize(String *pbuf, const size_t newcap) {
    if (pbuf == NULL || *pbuf == NULL)
        return false;

    String buf = *pbuf;

    if (newcap == buf->cap)
        return true;

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
 * @fn void string_move(String *to, String *from)
 * @brief Copy string and free from
 *
 * @param to Buffered string
 * @param from Buffered string
 */
uint32_t string_move(String *to, String *from) {
    if (to == NULL || from == NULL || *to == NULL || *from == NULL)
        return UINT32_MAX;

    if ((*from)->len > (*to)->len)
        if (!string_resize(to, (*from)->cap))
            return UINT32_MAX;

    memcpy((*to), (*from), BUF_MEM((*from)->len));
    (*to)->len = (*from)->len;
    free(*from);

    return 0;
}

/**
 * @fn uint32_t string_copy(String *to, const char *from)
 * @brief Copy string
 *
 * @param to Buffered string
 * @param from string
 */
uint32_t string_copy(String *to, const char *from) {
    if (to == NULL || *to == NULL || from == NULL)
        return UINT32_MAX;

    size_t lenf = strlen(from);
    if (lenf > UINT32_MAX - 1)
        return UINT32_MAX;

    if (lenf > (*to)->len)
        if (!string_resize(to, BUF_MEM(lenf)))
            return UINT32_MAX;

    memcpy((*to)->data, from, lenf + 1);
    (*to)->len = lenf;

    return 0;
}

/**
 * @fn const char* string_buf_data(const String buf)
 * @brief Return Data of Buffered string
 *
 * @param buf Buffered string
 * @return String
 */
const char* string_data(const String buf) {
    if (buf == NULL)
        return NULL;

    return buf->data;
}

/**
 * @fn void string_buf_reset(String buf)
 * @brief Reset Buffered string content
 *
 * @param buf Buffered string
 */
void string_reset(String buf) {
    if (buf == NULL)
        return;

    buf->len = 0;
    buf->data[0] = 0;
}

////////////////

String _str_result_tmp_xxxxxxx_; /**< for move macros >**/

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
 * @fn size_t string_buf_cap(const String buf)
 * @brief Return capacity
 *
 * @param buf Buffered string
 * @return Capacity
 */
size_t string_cap(const String buf) {
    if (buf == NULL)
        return UINT32_MAX;

    return buf->cap;
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

    String new = string_new(pos + 1);
    memcpy(new->data, buf->data, pos + 1);
    new->data[pos + 1] = '\0';
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

    String new = string_new(buf->len - pos);
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

    String new = string_new(right - left);
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

    String new = string_new(str1->len + str2->len);
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

    String new = string_new(buf->len + str->len);
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

    String new = string_new(buf->len - pos2 + pos1);
    memcpy(new->data, buf->data, pos1);
    memcpy(new->data + pos1, buf->data + pos2 + 1, buf->len - pos2);

    new->len = buf->len - pos2 + pos1 - 1;

    return new;
}

/**
 * @fn String string_delete_c(const String buf, const char *str)
 * @brief Delete substring str
 *
 * @param buf Buffered string
 * @param str string
 * @return Buffered string
 */
String string_delete_c(const String buf, const char *str) {
    if (buf == NULL || str == NULL) {
        return NULL;
    }

    uint32_t pos1 = string_find_c(buf, str, 0);
    if (pos1 == STR_ERROR)
        return NULL;

    uint32_t pos2 = pos1 + strlen(str) - 1;

    return string_delete(buf, pos1, pos2);
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

    String new = string_new(buf->len - search->len + replace->len);
    memcpy(new->data, buf->data, fpos);
    memcpy(new->data + fpos, replace->data, replace->len);
    memcpy(new->data + fpos + replace->len, buf->data + search->len + fpos, buf->len - fpos - search->len);

    new->len = buf->len - search->len + replace->len;

    return new;
}

/**
 * @fn String string_replace_c(const String buf, const char *search, const char *replace, uint32_t pos)
 * @brief Replace string
 *
 * @param buf Buffered string
 * @param search string
 * @param replace string
 * @param pos Start position
 * @return Buffered string
 */
String string_replace_c(const String buf, const char *c_search, const char *c_replace, uint32_t pos) {
    if (buf == NULL || c_search == NULL || c_replace == NULL || pos > buf->len)
        return NULL;

    String search = string_new_c(c_search);
    String replace = string_new_c(c_replace);

    uint32_t fpos = string_find(buf, search, pos);
    if (fpos == STR_ERROR)
        return NULL;

    String new = string_new(buf->len - search->len + replace->len);
    memcpy(new->data, buf->data, fpos);
    memcpy(new->data + fpos, replace->data, replace->len);
    memcpy(new->data + fpos + replace->len, buf->data + search->len + fpos, buf->len - fpos - search->len);

    new->len = buf->len - search->len + replace->len;

    free(search);
    free(replace);

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
 * @fn uint32_t string_find_c(const String buf, const char *csearch, uint32_t pos)
 * @brief Find character starting at position.
 *
 * @param buf Buffered string
 * @param csearch Searched string
 * @param pos Start position
 * @return Position
 */
uint32_t string_find_c(const String buf, const char *csearch, uint32_t pos) {
    if (buf == NULL || csearch == NULL || pos > buf->len)
        return false;

    String search = string_new_c(csearch);
    uint32_t p = string_find(buf, search, pos);
    free(search);

    return p;
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

    String new = string_new(buf->len);
    for (int i = 0; i < buf->len; i++) {
        if (buf->data[i] >= 97 && buf->data[i] <= 122)
            new->data[i] = buf->data[i] - 32;
        else
            new->data[i] = buf->data[i];
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

    String new = string_new(buf->len);
    for (int i = 0; i < buf->len; i++) {
        if (buf->data[i] >= 65 && buf->data[i] <= 90)
            new->data[i] = buf->data[i] + 32;
        else
            new->data[i] = buf->data[i];
    }

    new->len = buf->len;

    return new;
}

/**
 * @fn String string_ltrim(const String buf)
 * @brief Left trim string
 *
 * @param buf Buffered string
 * @return Buffered string
 */
String string_ltrim(const String buf) {
    if (buf == NULL)
        return NULL;

    uint32_t pos1 = 0;

    while (pos1 < buf->len && isspace(buf->data[pos1]))
        ++pos1;

    String new = string_new(pos1 + (buf->len - 1) + 1);
    memcpy(new->data, buf->data + pos1, (buf->len - 1) - pos1 + 1);

    new->len = (buf->len - 1) - pos1 + 1;

    return new;
}

/**
 * @fn String string_rtrim(const String buf)
 * @brief Right trim string
 *
 * @param buf Buffered string
 * @return Buffered string
 */
String string_rtrim(const String buf) {
    if (buf == NULL)
        return NULL;

    uint32_t pos2 = buf->len - 1;

    while (pos2 >= 0 && isspace(buf->data[pos2]))
        --pos2;

    String new = string_new(pos2 + 1);
    memcpy(new->data, buf->data, pos2 + 1);

    new->len = pos2 + 1;

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

    while (pos1 < buf->len && isspace(buf->data[pos1]))
        ++pos1;
    while (pos2 >= 0 && isspace(buf->data[pos2]))
        --pos2;

    String new = string_new(pos1 + pos2 + 1);
    memcpy(new->data, buf->data + pos1, pos2 - pos1 + 1);

    new->len = pos2 - pos1 + 1;

    return new;
}

/**
 * @fn int string_append(String buf, const char *fmt, ...)
 * @brief Append a formatted c-string to `buf`.
 *        If new data would exceed capacity, `buf` stays unmodified.
 *
 * @param buf  Buffered string
 * @param fmt Format
 * @return Change in length.
 */
int string_append(String buf, const char *fmt, ...) {
    if (buf == NULL || fmt == NULL)
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
 * @fn int string_write(String buf, const char *fmt, ...)
 * @brief Write a formatted c-string at beginning of `buf`.
 *        If new data would exceed capacity, `buf` stays unmodified.
 *
 * @param buf  Buffered string
 * @param fmt Format
 * @return New length or zero on failure.
 */
int string_write(String buf, const char *fmt, ...) {
    if (buf == NULL || fmt == NULL)
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

    return !memcmp(str1->data, str2->data, str1->len);
}

/**
 * @fn bool string_equals_c(const String a, const char *b)
 * @brief Compare strings equality
 *
 * @param a Buffered string
 * @param b String
 * @return Boolean
 */
bool string_equals_c(const String a, const char *b) {
    if (a == NULL || b == NULL || a->len != strlen(b))
        return false;

    return !memcmp(a->data, b, a->len);
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

/**
 * @fn String string_split(const String buf, const char *search, const String *right)
 * @brief Split string and return left and right Strings
 *
 * @param buf Buffered string
 * @param search string to search
 * @param right Buffered string
 * @return String Left Buffered string
 */
String string_split(const String buf, const char *search, String *right) {
    if (buf == NULL || search == NULL)
        return NULL;

    uint32_t pos = string_find_c(buf, search, 0);
    if(pos == STR_ERROR)
        return NULL;

    *right = string_right(buf, pos + strlen(search));

    return string_left(buf, pos - 1);
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
    else
        halfsiphash(buf->data, buf->len, key, result.out, len);

    return result;
}